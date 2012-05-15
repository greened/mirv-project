#ifndef mirv_Core_Builder_ConstructTransform_hpp
#define mirv_Core_Builder_ConstructTransform_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Builder/ConstantSymbolTransforms.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>
#include <mirv/Core/Builder/Fusion.hpp>
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
    /// Transform a one-operand node into a single-child IR node.
    template<typename NodeType,
      typename Child = typename NodeType::ChildPtr,
      typename Dummy = boost::proto::callable>
    struct ConstructUnary : boost::proto::callable {
      typedef ptr<NodeType> result_type;

      result_type operator()(boost::shared_ptr<const SymbolTable> symtab,
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
      typedef ptr<NodeType> result_type;

      result_type operator()(boost::shared_ptr<const SymbolTable> symtab,
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
      typedef ptr<Statement<Block> > result_type;

      result_type operator()(boost::shared_ptr<const SymbolTable> symtab,
                             Statement<Block>::ChildPtr left,
                             Statement<Block>::ChildPtr right) {
        if (ptr<Statement<Block> > lb =
            dyn_cast<Statement<Block> >(left)) {
          if (ptr<Statement<Block> > rb =
              dyn_cast<Statement<Block> >(right)) {
            std::copy(rb->begin(), rb->end(), std::back_inserter(*lb));
          }
          lb->push_back(right);
          return lb;
        }
        else if (ptr<Statement<Block> > rb =
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
      typedef ptr<NodeType> result_type;

      result_type operator()(boost::shared_ptr<const SymbolTable> symtab,
                             Child1 child1, Child2 child2, Child3 child3) {
        return make<NodeType>(child1, child2, child3);
      }
    };

    /// This is a grammar action to construct an N-ary expression.
    template<typename NodeType, typename Dummy = boost::proto::callable>
    struct ConstructNary : boost::proto::callable {
      typedef ptr<NodeType> result_type;
        
      template<typename FusionSequence>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             ptr<Expression<Base> > a1,
                             const FusionSequence &expr) {
        std::vector<ptr<Expression<Base> >> operands;
        detail::PopAndTranslateFusionExpressionSequence()(
          symtab, expr, std::back_inserter(operands));
        return mirv::make<NodeType>(a1, operands.begin(), operands.end());
      }

      template<typename Arg1, typename Arg2, typename FusionSequence>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg1 a1,
                             Arg2 a2,
                             const FusionSequence &expr) {
        std::vector<ptr<Expression<Base> >> operands;
        detail::PopAndTranslateFusionExpressionSequence()(
          symtab, expr, std::back_inserter(operands));
        return mirv::make<NodeType>(a1, a2, operands.begin(), operands.end());
      }
    };

    // A transform to flatten an expression tree and process it as a
    // fusion sequence.
    template<typename NodeType, typename Dummy = boost::proto::callable>
    struct ConstructNaryFlat : boost::proto::callable {
      typedef ptr<NodeType> result_type;

      template<typename Arg1, typename FusionSequence>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg1 a1,
                             const FusionSequence &expr) {
        std::vector<ptr<Expression<Base> >> operands;
        detail::FlattenAndTranslateFusionExpressionSequence()(
          symtab, expr, std::back_inserter(operands));
        return mirv::make<NodeType>(a1, operands.begin(), operands.end());
      }

      template<typename Arg1, typename Arg2, typename FusionSequence>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg1 a1,
                             Arg2 a2,
                             const FusionSequence &expr) {
        std::vector<ptr<Expression<Base> >> operands;
        detail::FlattenAndTranslateFusionExpressionSequence()(
          symtab, expr, std::back_inserter(operands));
        return mirv::make<NodeType>(a1, a2, operands.begin(), operands.end());
      }
    };

    namespace detail {
      /// Given a value which should be a load, return the address
      /// loaded from.  If the load produces an address, return an
      /// empty result.
      ptr<Expression<Base> >
      extractLoadAddress(boost::shared_ptr<SymbolTable> symtab,
                         ptr<Expression<Base> > value);

      /// Given a value to store to, obtain its address to feed to a
      /// store operation.
      ptr<Expression<Base> >
      extractStoreAddress(boost::shared_ptr<SymbolTable> symtab,
                          ptr<Expression<Base> > value);
    }

    /// Given a sequnce of indices, construct a tuple pointer.
    struct ConstructAddress : boost::proto::callable {
      typedef ptr<Expression<Base> > result_type;
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             ptr<Expression<Base> > base,
                             ptr<Expression<Base> > index);
    };

    /// Given a sequnce of indices, construct a tuple pointer.
    struct ConstructAddressFromSequence : boost::proto::callable {
      typedef ptr<Expression<Base> > result_type;

      template<typename Sequence>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             ptr<Expression<Base> > base,
                             Sequence indices) {
        result_type basePointer = detail::extractLoadAddress(symtab, base);
        if (basePointer) {
          ptr<Expression<Base> > offset =
            Expression<Reference<Constant<Base> > >::make(
              ConstructIntegerConstantSymbol<0>()(symtab));
          return ConstructNaryFlat<Expression<TuplePointer> >()(symtab,
                                                                basePointer,
                                                                offset,
                                                                indices);
        }
        // Base is already a pointer.  Don't add an extra zero offset
        // because the indices include it.
        return ConstructNaryFlat<Expression<TuplePointer> >()(symtab,
                                                              base,
                                                              indices);
      }
    };
  }
}

#endif
