#ifndef mirv_Core_Builder_Transform_hpp
#define mirv_Core_Builder_Transform_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/TranslateFwd.hpp>
#include <mirv/Core/IR/Control.hpp>

#include <boost/proto/proto.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>

#include <iterator>
#include <algorithm>

namespace mirv {
  namespace Builder {
    // Bundle any pending statements created from child expressions
    // with this statement.
    struct ClearPendingStatements : boost::proto::callable {
      typedef ptr<Statement<Base> >::type StatementPointer;
      typedef StatementPointer result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             StatementPointer stmt);
    };

    // Loops need to be handled specially.
    struct ClearPendingStatementsDoWhile : boost::proto::callable {
      typedef ptr<Statement<DoWhile> >::type StatementPointer;
      typedef StatementPointer result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             StatementPointer stmt);
    };

    // Since there is no while statement, indicate a while by
    // specializing on the grammar rule.
    struct WhileRule;

    struct ClearPendingStatementsWhileRule : boost::proto::callable {
      typedef ptr<Statement<IfThen> >::type StatementPointer;
      typedef StatementPointer result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             StatementPointer stmt);
    };

    /// Transform a one-operand node into a single-child IR node.
    template<typename NodeType,
      typename Child = typename NodeType::ChildPtr,
      typename Dummy = boost::proto::callable>
    struct ConstructUnary : boost::proto::callable {
      typedef typename ptr<NodeType>::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Child child) {
        return make<NodeType>(child);
      }
    };
  
    /// Transform a two-operand node to a two-child IR node.
    template<typename NodeType,
      typename Child1 = typename NodeType::ChildPtr,
      typename Child2 = typename NodeType::ChildPtr,
      typename Dummy = boost::proto::callable>
    struct ConstructBinary : boost::proto::callable {
      typedef typename ptr<NodeType>::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Child1 left, Child2 right) {
        return make<NodeType>(left, right);
      }
    };

    /// This is a specialization for block statements to add the
    /// child to a block if it already exists.
    template<>
    struct ConstructBinary<Statement<Block>,
      Statement<Block>::ChildPtr,
      Statement<Block>::ChildPtr,
      boost::proto::callable> : boost::proto::callable {
      typedef ptr<Statement<Block> >::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Statement<Block>::ChildPtr left,
                             Statement<Block>::ChildPtr right) {
        if (ptr<Statement<Block> >::type lb =
            dyn_cast<Statement<Block> >(left)) {
          if (ptr<Statement<Block> >::type rb =
              dyn_cast<Statement<Block> >(right)) {
            std::copy(rb->begin(), rb->end(), std::back_inserter(*lb));
          }
          lb->push_back(right);
          return lb;
        }
        else if (ptr<Statement<Block> >::type rb =
                 dyn_cast<Statement<Block> >(right)) {
          rb->push_front(left);
          return rb;
        }
        return make<Statement<Block> >(left, right);
      }
    };

    /// Transform a three-operand node to a three-child IR node.
    template<typename NodeType,
      typename Child1 = typename NodeType::ChildPtr,
      typename Child2 = typename NodeType::ChildPtr,
      typename Child3 = typename NodeType::ChildPtr,
      typename Dummy = boost::proto::callable>
    struct ConstructTernary : boost::proto::callable {
      typedef typename ptr<NodeType>::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Child1 child1, Child2 child2, Child3 child3) {
        return make<NodeType>(child1, child2, child3);
      }
    };
  
    /// This is a callable transform to translate a proto expression
    /// to a mirv expression.
    template<typename ExpressionType>
    class TranslateToExpression : boost::proto::callable {
    private:
      boost::shared_ptr<SymbolTable> symtab;

    public:
      TranslateToExpression<ExpressionType>(boost::shared_ptr<SymbolTable> s)
      : symtab(s) {}

      typedef typename ptr<ExpressionType>::type result_type;

      template<typename Expr>
      result_type operator()(const Expr &e) const {
        //std::cout << "Translating:\n";
        //boost::proto::display_expr(e);
        return safe_cast<ExpressionType>(
          translateWithGrammar<ConstructExpressionGrammar>(e, symtab));
      }
    };

    // Transform for an n-ary expression
    template<typename NodeType, typename Dummy = boost::proto::callable>
    struct ConstructNary : boost::proto::callable {
      typedef typename ptr<NodeType>::type result_type;
        
      template<typename Arg1, typename Arg2>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg1 a1,
                             const Arg2 &a2) {
        TranslateToExpression<Expression<Base>> translator(symtab);
        return mirv::make<NodeType>(a1,
                                    boost::fusion::transform(
                                      boost::fusion::pop_front(a2),
                                      translator));
      }

      template<typename Arg1, typename Arg2, typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg1 a1,
                             Arg2 a2,
                             const Expr &expr) {
        TranslateToExpression<Expression<Base>> translator(symtab);
        return mirv::make<NodeType>(a1,
                                    a2,
                                    boost::fusion::transform(
                                      boost::fusion::pop_front(expr),
                                      translator));
      }
    };

    // A transform to flatten an expression tree and process it as a
    // fusion sequence.
    template<typename NodeType, typename Dummy = boost::proto::callable>
    struct ConstructNaryFlat : boost::proto::callable {
      typedef typename ptr<NodeType>::type result_type;
        
      template<typename Arg1, typename Arg2>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg1 a1,
                             const Arg2 &a2) {
        TranslateToExpression<Expression<Base>> translator(symtab);
        return mirv::make<NodeType>(a1,
                                    boost::fusion::transform(
                                      boost::proto::flatten(a2), translator));
      }

      template<typename Arg1, typename Arg2, typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg1 a1,
                             Arg2 a2,
                             const Expr &expr) {
        TranslateToExpression<Expression<Base>> translator(symtab);
        return mirv::make<NodeType>(a1,
                                    a2,
                                    boost::fusion::transform(
                                      expr,
                                      translator));
      }
    };
  }
}

#endif
