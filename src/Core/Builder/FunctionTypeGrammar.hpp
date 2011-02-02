#ifndef mirv_Core_Builder_FunctionTypeGrammar_hpp
#define mirv_Core_Builder_FunctionTypeGrammar_hpp

#include <mirv/Core/Builder/FunctionTypeGrammarFwd.hpp>

#include <mirv/Core/Builder/VoidTypeGrammar.hpp>
#include <mirv/Core/Builder/TypeGrammar.hpp>
#include <mirv/Core/Builder/FunctionTypeRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/proto/proto.hpp>
#include <boost/mpl/print.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/type_traits.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      /// This is a helper class so we don't do illegal fusion
      /// operations when we don't have a vararg specifier.
      template<bool Vararg>
      struct Helper {
        typedef ptr<Symbol<Type<FunctionType> > >::const_type result_type;
        template<typename Arg1, typename Arg2>
        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               Arg1 a1,
                               const Arg2 &a2) {
        TranslateToSymbol<Symbol<Type<TypeBase> > > translator(symtab);
        return TernaryConstructSymbol<Symbol<Type<FunctionType> > >()(
          symtab, a1, boost::fusion::transform(
            boost::fusion::pop_front(a2), translator),
          Symbol<Type<FunctionType> >::VarargMark::NotVararg);
        }
      };

      template<>
      struct Helper<true> {
        typedef ptr<Symbol<Type<FunctionType> > >::const_type result_type;
        template<typename Arg1, typename Arg2>
        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               Arg1 a1,
                               const Arg2 &a2) {
          TranslateToSymbol<Symbol<Type<TypeBase> > > translator(symtab);
          return TernaryConstructSymbol<Symbol<Type<FunctionType> > >()(
            symtab,
            a1,
            boost::fusion::transform(
              boost::fusion::pop_front(
                boost::fusion::pop_back(a2)),
              translator),
            Symbol<Type<FunctionType>>::VarargMark::Vararg);
        }
      };
    }
    
    /// This is a callable transform to construct a function type.
    struct ConstructFunctionTypeSymbol : boost::proto::callable {
      typedef ptr<Symbol<Type<FunctionType> > >::const_type result_type;

      template<typename Arg1, typename Arg2>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
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

    struct FunctionTypeBuilder : boost::proto::when<
      FunctionTypeRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        ConstructFunctionTypeSymbol(
          boost::proto::_data,
          FunctionReturnTypeBuilder(boost::proto::_left),
          boost::proto::_expr))
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
