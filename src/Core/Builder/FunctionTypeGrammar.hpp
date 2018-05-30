#ifndef mirv_Core_Builder_FunctionTypeGrammar_hpp
#define mirv_Core_Builder_FunctionTypeGrammar_hpp

#include <mirv/Core/Builder/FunctionTypeGrammarFwd.hpp>

#include <mirv/Core/Builder/VoidTypeGrammar.hpp>
#include <mirv/Core/Builder/TypeGrammar.hpp>
#include <mirv/Core/Builder/FunctionTypeRules.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>
#include <mirv/Core/Builder/Fusion.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/proto/proto.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/mpl/print.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/type_traits.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      /// This is a helper class so we don't do illegal fusion
      /// operations when we don't have a vararg specifier.  It
      /// handles the case where we do not have a vararg specifier.
      template<bool Vararg>
      struct Helper {
        typedef ptr<const FunctionType> result_type;
        template<typename Arg1, typename FusionSequence>
        result_type operator()(ptr<SymbolTable> symtab,
                               Arg1 a1,
                               const FusionSequence &expr) {
          auto Build = [a1] (auto ...args) -> ptr<const FunctionType> {
            return IRBuilder::
            getFunctionType(FunctionType::VarargMark::NotVararg,
                            fast_cast<const Type>(std::move(a1)),
                            fast_cast<const Type>(std::move(args))...);
          };

          using boost::fusion::invoke;
          using boost::fusion::transform;
          using boost::fusion::pop_front;

          TranslateToSymbol<const Type> translator(symtab);
          return invoke(Build,
                        transform(pop_front(expr),
                                  translator));
          // std::vector<ptr<const Type>> operands;
          // PopAndTranslateFusionTypeSequence()(symtab, expr, std::back_inserter(operands));
          // return QuaternaryConstructSymbol<FunctionType, ModuleScope>()(
          //   symtab, a1, operands.begin(), operands.end(),
          //   FunctionType::VarargMark::NotVararg);
        }
      };

      /// This is a helper class so we don't do illegal fusion
      /// operations when we don't have a vararg specifier.  It
      /// handles the case where we do have a vararg specifier.
      template<>
      struct Helper<true> {
        typedef ptr<const FunctionType> result_type;
        template<typename Arg1, typename FusionSequence>
        result_type operator()(ptr<SymbolTable> symtab,
                               Arg1 a1,
                               const FusionSequence &expr) {
          auto Build = [a1] (auto ...args) -> ptr<const FunctionType> {
            return IRBuilder::
            getFunctionType(FunctionType::VarargMark::Vararg,
                            std::move(a1),
                            std::move(args)...);
          };

          using boost::fusion::invoke;
          using boost::fusion::transform;
          using boost::fusion::pop_front;
          using boost::fusion::pop_back;

          TranslateToSymbol<const Type> translator(symtab);
          return invoke(Build,
                        transform(pop_front(pop_back(expr)),
                                  translator));
          // std::vector<ptr<const Type>> operands;
          // PopFrontBackAndTranslateFusionTypeSequence()(symtab, expr, std::back_inserter(operands));

          // return QuaternaryConstructSymbol<FunctionType, ModuleScope>()(
          //   symtab, a1, operands.begin(), operands.end(),
          //   FunctionType::VarargMark::Vararg);
        }
      };
    }

    /// This is a callable transform to construct a function type.
    struct ConstructFunctionTypeSymbol : boost::proto::callable {
      typedef ptr<const FunctionType> result_type;

      template<typename Arg1, typename Arg2>
      result_type operator()(ptr<SymbolTable> symtab,
                             Arg1 a1,
                             const Arg2 &a2) {
        // Check vararg information.
        return detail::Helper<
          std::is_same<
            VarargTerminal,
            typename std::remove_const<
              typename std::remove_reference<
                typename boost::fusion::result_of::back<Arg2>::type
                >::type
              >::type
            >::value
          >()(symtab, a1, a2);
      }
    };

    /// This is the grammar for function types.
    struct FunctionReturnTypeBuilder : boost::proto::or_<
      VoidBuilder,
      TypeBuilder
      > {};

    /// This is the grammar for looking up function return types.
    struct FunctionReturnTypeLookupBuilder : boost::proto::or_<
      VoidBuilder,
      TypeLookupBuilder
      > {};

    // struct FunctionTypeBuilder : boost::proto::or_<
    //   boost::proto::when<
    //     FunctionTypeWithArgsRule,
    //     LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
    //       boost::proto::_data,
    //       ConstructFunctionTypeSymbol(
    //         boost::proto::_data,
    //         FunctionReturnTypeBuilder(boost::proto::_left),
    //         boost::proto::_expr))
    //     >,
    //   boost::proto::when<
    //     FunctionTypeWithoutArgsRule,
    //     LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
    //       boost::proto::_data,
    //       UnaryConstructSymbol<Symbol<Type<FunctionType> > >(
    //         boost::proto::_data,
    //         FunctionReturnTypeBuilder(boost::proto::_left)))
    //     >
    //   > {};

    /// Given a function type specification, return its corresponding
    /// Symbol.
    struct FunctionTypeBuilder : boost::proto::when<
      FunctionTypeRule,
      //      LookupAndAddSymbol<Type>(
      //        boost::proto::_data,
        ConstructFunctionTypeSymbol(
          boost::proto::_data,
          FunctionReturnTypeBuilder(boost::proto::_left),
          boost::proto::_expr)
      //)
      > {};

    /// This is the grammar to lookup function types.  It is almost
    /// the same as the construct grammar except for struct types it
    /// assumes the struct already exists.  This lets the rule avoid
    /// creating and returning placeholders instead of the real type.
    struct FunctionTypeLookupBuilder : boost::proto::when<
      FunctionTypeRule,
      //LookupAndAddSymbol<Type>(
      //boost::proto::_data,
        ConstructFunctionTypeSymbol(
          boost::proto::_data,
          FunctionReturnTypeLookupBuilder(boost::proto::_left),
          // FIXME: We probably should do lookups here.  If we pass
          // structs this will break.  We can do something like what's
          // done for array types except translateToSymbol uses
          // ConstructSymbolGrammar.  Possibly specialize this for
          // types?
          boost::proto::_expr)
      //)
      > {};

    namespace {
      /// A vararg "operator."  This is a protoized object that
      /// implements the function operator to construct a vararg
      /// parameter.
      const VarargTerminal vararg = {{}};
    }
  }
}

#endif
