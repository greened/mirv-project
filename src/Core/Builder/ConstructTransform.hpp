#ifndef mirv_Core_Builder_ConstructTransform_hpp
#define mirv_Core_Builder_ConstructTransform_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ConstantSymbolTransforms.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>
#include <mirv/Core/Builder/Fusion.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <boost/proto/proto.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/range/algorithm/copy.hpp>

#include <iterator>
#include <algorithm>

namespace mirv {
  namespace Builder {
    /// Transform a one-operand node into a single-child IR node.
    template<typename NodeType,
             typename Child = ptr<ValueProducer>,
      typename Dummy = boost::proto::callable>
    struct ConstructUnary : boost::proto::callable {
      typedef ptr<NodeType> result_type;

      result_type operator()(ptr<const SymbolTable> symtab, Child child) {
        std::cerr << "ConstructUnary\n";
        return IRBuilder::get<NodeType>(child);
      }
    };

    /// Transform a two-operand node to a two-child IR node.
    template<typename NodeType,
             typename Child1 = ptr<ValueProducer>,
             typename Child2 = ptr<ValueProducer>,
             typename Dummy = boost::proto::callable>
    struct ConstructBinary : boost::proto::callable {
      typedef ptr<NodeType> result_type;

      result_type operator()(ptr<const SymbolTable> symtab,
                             Child1 left, Child2 right) {
        std::cerr << "ConstructBinary\n";
        return IRBuilder::get<NodeType>(left, right);
      }
    };

    /// This is a specialization for block statements to add the
    /// child to a block if it already exists.
    template<>
    struct ConstructBinary<Block,
                           ptr<Control>,
                           ptr<Control>,
                           boost::proto::callable> : boost::proto::callable {
      typedef ptr<Block> result_type;

      result_type operator()(ptr<const SymbolTable> symtab,
                             ptr<Control> left,
                             ptr<Control> right) {
        std::cerr << "ConstructBinary(Block)\n";
        if (ptr<Block> lb = dyn_cast<Block>(left)) {
          if (ptr<Block> rb = dyn_cast<Block>(right)) {
            boost::range::copy(rb->Statements(), std::back_inserter(*lb));
            rb->clear();
            rb.destroy();
            return lb;
          }
          lb->push_back(right);
          return lb;
        }
        else if (ptr<Block> rb = dyn_cast<Block>(right)) {
          rb->push_front(left);
          return rb;
        }
        return IRBuilder::get<Block>(left, right);
      }
    };

    /// Transform a three-operand node to a three-child IR node.
    template<typename NodeType,
             typename Child1 = ptr<ValueProducer>,
             typename Child2 = ptr<ValueProducer>,
             typename Child3 = ptr<ValueProducer>,
             typename Dummy = boost::proto::callable>
    struct ConstructTernary : boost::proto::callable {
      typedef ptr<NodeType> result_type;

      result_type operator()(ptr<const SymbolTable> symtab,
                             Child1 child1, Child2 child2, Child3 child3) {
        std::cerr << "ConstructTernary\n";
        return IRBuilder::get<NodeType>(child1, child2, child3);
      }
    };

    /// This is a grammar action to construct an N-ary expression.
    template<typename NodeType, typename Dummy = boost::proto::callable>
    struct ConstructNary : boost::proto::callable {
      typedef ptr<NodeType> result_type;

      template<typename FusionSequence>
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<ValueProducer> a1,
                             const FusionSequence &expr) {
        std::cerr << "ConstructNary(1)\n";
        auto Build = [a1] (auto ...args) -> ptr<NodeType> {
          return IRBuilder::get<NodeType>(std::move(a1), std::move(args)...);
        };

        using boost::fusion::invoke;
        using boost::fusion::transform;
        using boost::fusion::pop_front;

        TranslateToExpression<ValueProducer> translator(symtab);
        return invoke(Build,
                      transform(pop_front(expr),
                                translator));
        // std::vector<ptr<ValueProducer>> operands;
        // detail::PopAndTranslateFusionExpressionSequence()(
        //   symtab, expr, std::back_inserter(operands));
        // return IRBuilder::get<NodeType>(a1, operands.begin(), operands.end());
      }

      template<typename Arg1, typename Arg2, typename FusionSequence>
      result_type operator()(ptr<SymbolTable> symtab,
                             Arg1 a1,
                             Arg2 a2,
                             const FusionSequence &expr) {
        std::cerr << "ConstructNary(2)\n";
        auto Build = [a1, a2] (auto ...args) -> ptr<NodeType> {
          return IRBuilder::get<NodeType>(std::move(a1),
                                          std::move(a2),
                                          std::move(args)...);
        };

        using boost::fusion::invoke;
        using boost::fusion::transform;
        using boost::fusion::pop_front;

        TranslateToExpression<ValueProducer> translator(symtab);
        return invoke(Build,
                      transform(pop_front(expr),
                                translator));
        // std::vector<ptr<ValueProducer>> operands;
        // detail::PopAndTranslateFusionExpressionSequence()(
        //   symtab, expr, std::back_inserter(operands));
        // return IRBuilder::get<NodeType>(a1, a2, operands.begin(), operands.end());
      }
    };

    // A transform to flatten an expression tree and process it as a
    // fusion sequence.
    template<typename NodeType, typename Dummy = boost::proto::callable>
    struct ConstructNaryFlat : boost::proto::callable {
      typedef ptr<NodeType> result_type;

      template<typename Arg1, typename FusionSequence>
      result_type operator()(ptr<SymbolTable> symtab,
                             Arg1 a1,
                             const FusionSequence &expr) {
        std::cerr << "ConstructNaryFlat(1)\n";
        auto Build = [a1] (auto ...args) -> ptr<NodeType> {
          return IRBuilder::get<NodeType>(std::move(a1), std::move(args)...);
        };

        using boost::fusion::invoke;
        using boost::fusion::transform;
        using boost::proto::flatten;

        TranslateToExpression<ValueProducer> translator(symtab);
        return invoke(Build,
                      transform(flatten(expr),
                                translator));
        // std::vector<ptr<ValueProducer>> operands;
        // detail::FlattenAndTranslateFusionExpressionSequence()(
        //   symtab, expr, std::back_inserter(operands));
        // return IRBuilder::get<NodeType>(a1, operands.begin(), operands.end());
      }

      template<typename Arg1, typename Arg2, typename FusionSequence>
      result_type operator()(ptr<SymbolTable> symtab,
                             Arg1 a1,
                             Arg2 a2,
                             const FusionSequence &expr) {
        std::cerr << "ConstructNaryFlat(2)\n";
        auto Build = [a1, a2] (auto ...args) -> ptr<NodeType> {
          return IRBuilder::get<NodeType>(std::move(a1),
                                          std::move(a2),
                                          std::move(args)...);
        };

        using boost::fusion::invoke;
        using boost::fusion::transform;
        using boost::proto::flatten;

        TranslateToExpression<ValueProducer> translator(symtab);
        return invoke(Build,
                      transform(flatten(expr),
                                translator));
        // std::vector<ptr<ValueProducer>> operands;
        // detail::FlattenAndTranslateFusionExpressionSequence()(
        //   symtab, expr, std::back_inserter(operands));
        // return IRBuilder::get<NodeType>(a1, a2, operands.begin(), operands.end());
      }
    };

    namespace detail {
      /// Given a value which should be a load, return the address
      /// loaded from.  If the load produces an address, return an
      /// empty result.
      ptr<ValueProducer>
      extractLoadAddress(ptr<SymbolTable> symtab, ptr<ValueProducer> value);

      /// Given a value to store to, obtain its address to feed to a
      /// store operation.
      ptr<ValueProducer>
      extractStoreAddress(ptr<SymbolTable> symtab, ptr<ValueProducer> value);
    }

    /// Given a sequnce of indices, construct a tuple pointer.
    struct ConstructAddress : boost::proto::callable {
      typedef ptr<ValueProducer> result_type;
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<ValueProducer> base,
                             ptr<ValueProducer> index);
    };

    /// Given a sequnce of indices, construct a tuple pointer.
    struct ConstructAddressFromSequence : boost::proto::callable {
      typedef ptr<ValueProducer> result_type;

      template<typename Sequence>
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<ValueProducer> base,
                             Sequence indices) {
        std::cerr << "ConstructAddressFromSequence\n";
        result_type basePointer = detail::extractLoadAddress(symtab, base);
        if (basePointer) {
          ptr<ValueProducer> offset =
            ConstructIntegerConstantSymbol<0>()(symtab);
          return ConstructNaryFlat<TuplePointer>()(symtab,
                                                   basePointer,
                                                   offset,
                                                   indices);
        }
        // Base is already a pointer.  Don't add an extra zero offset
        // because the indices include it.
        return ConstructNaryFlat<TuplePointer>()(symtab,
                                                 base,
                                                 indices);
      }
    };
  }
}

#endif
