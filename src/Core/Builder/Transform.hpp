#ifndef mirv_Core_Builder_Transform_hpp
#define mirv_Core_Builder_Transform_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Builder/ConstantSymbolTransforms.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/TranslateFwd.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Type.hpp>

#include <boost/proto/proto.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>

#include <iterator>
#include <algorithm>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to add a statement to a function.
    struct AddAllocateStatement : boost::proto::callable {
      typedef ptr<Statement<Base> >::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             boost::shared_ptr<Statement<Base> > stmt);
    };
    
    /// Get a variable symbol given a statement.
    struct ExtractVariable : boost::proto::callable {
      typedef ptr<Symbol<Variable> >::type VariablePointer;
      typedef VariablePointer result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             ptr<Statement<Base> >::type stmt);
    };

    /// Bundle any pending statements created from child expressions
    /// with the statement just processed.
    struct ClearPendingStatements : boost::proto::callable {
      typedef ptr<Statement<Base> >::type StatementPointer;
      typedef StatementPointer result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             StatementPointer stmt);
    };

    /// This is a grammar action to output statements generated as a
    /// side-effect of translating do-while condition expressions.
    /// These statements need to be placed in the loop body
    /// immediately before the condition.
    struct ClearPendingStatementsDoWhile : boost::proto::callable {
      typedef ptr<Statement<DoWhile> >::type StatementPointer;
      typedef StatementPointer result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             StatementPointer stmt);
    };

    // Since there is no while statement, indicate a while by
    // specializing on the grammar rule.
    struct WhileRule;

    /// Group any statements from subexpresissions of the current
    /// statement with the current statement.  This handles things
    /// like function calls in the condition expression.
    struct ClearPendingStatementsWhileRule : boost::proto::callable {
      typedef ptr<Statement<IfThen> >::type StatementPointer;
      typedef StatementPointer result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             StatementPointer stmt);
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
  }
}

#endif
