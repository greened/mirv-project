#ifndef mirv_Core_Builder_ConstantGrammar_hpp
#define mirv_Core_Builder_ConstantGrammar_hpp

#include <mirv/Core/Builder/ConstantRules.hpp>
#include <mirv/Core/Builder/ConstantSymbolTransforms.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      struct IntegralTypeGen {
        typedef ptr<Symbol<Type<TypeBase> > >::const_type result_type;

        result_type operator()(ptr<SymbolTable>::type symtab,
                               size_t bitsize);
      };
 
      struct FloatingTypeGen {
        typedef ptr<Symbol<Type<TypeBase> > >::const_type result_type;

        result_type operator()(ptr<SymbolTable>::type symtab,
                               size_t bitsize);
      };

      struct StringTypeGen {
        typedef ptr<Symbol<Type<TypeBase> > >::const_type result_type;

        result_type operator()(ptr<SymbolTable>::type symtab,
                               const std::string &value);
      };

      /// This is a callable transform to take a string-type variable
      /// reference and return the address of the first element, to
      /// give C semantics.
      struct GetCStringReference : boost::proto::callable {
        typedef ptr<Expression<Base> >::type result_type;

        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<Expression<Base> >::type str);
      };
    }

    /// This is a callable transform to add a string constant to a
    /// module.
    struct AddStringConstant : boost::proto::callable {
      typedef ptr<Expression<Base> >::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             ptr<Symbol<Constant<Base> > >::type str);
    };

    /// This is a callable transform to construct a constant symbol.
    /// If the symbol exists at the current module, it is an error.
    /// This specialization handles string constants since we
    /// calculate the size of a string differently.  "sizeof" won't
    /// work to get the length of a std::string.
    template<>
    struct ConstructConstantSymbol<detail::StringTypeGen,
      boost::proto::callable>
        : boost::proto::callable {
      typedef ptr<Symbol<Constant<Base>>>::type result_type;

      template<typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     const Expr &expr) {
        detail::StringTypeGen typeGen;

        // Constant type
        ptr<Symbol<Type<TypeBase> > >::const_type constantType = 
          LookupAndAddSymbol<Symbol<Type<TypeBase> > >()(
            symtab, typeGen(symtab, boost::proto::value(expr)));

        typedef Constant<std::string> ConstantType;

        result_type result =
          mirv::make<Symbol<ConstantType>>(constantType,
                                           boost::proto::value(expr));

	return result;
      }
    };

    /// This is the grammar for constant symbols.
    struct ConstantBuilder : boost::proto::or_<
      boost::proto::when<
        IntegralConstantRule,
        ConstructConstantSymbol<detail::IntegralTypeGen>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)
            >,
      boost::proto::when<
        FloatingConstantRule,
        ConstructConstantSymbol<detail::FloatingTypeGen>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)
        >
      > {};
    struct StringRefBuilder : boost::proto::when<
      StringConstantRule,
      detail::GetCStringReference(
        boost::proto::_data,
        AddStringConstant(
          boost::proto::_data,
          ConstructConstantSymbol<detail::StringTypeGen>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)))
      > {};
  }
}

#endif
