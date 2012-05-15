#ifndef mirv_Core_Builder_ConstantSymbolTransforms_hpp
#define mirv_Core_Builder_ConstantSymbolTransforms_hpp

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/ConstantFwd.hpp>
#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      struct IntegralTypeGen {
        typedef ptr<const Symbol<Type<TypeBase> > > result_type;
        typedef std::uint64_t value_type;

        result_type operator()(ptr<SymbolTable> symtab,
                               size_t bitsize);
      };
 
      /// This is a functor to generate a floating point type of some
      /// bitsize.
      struct FloatingTypeGen {
        typedef ptr<const Symbol<Type<TypeBase> > > result_type;
        typedef double value_type;

        result_type operator()(ptr<SymbolTable> symtab,
                               size_t bitsize);
      };

      /// This is a grammar action to generate the type for a C-style
      /// string.
      struct StringTypeGen {
        typedef ptr<const Symbol<Type<TypeBase> > > result_type;

        result_type operator()(ptr<SymbolTable> symtab,
                               const std::string &value);
      };

      /// This is a callable transform to take a string-type variable
      /// reference and return the address of the first element, to
      /// give C semantics.
      struct GetCStringReference : boost::proto::callable {
        typedef ptr<Expression<Base> > result_type;

        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               ptr<Expression<Base> > str);
      };
    }

    /// This is a callable transform to add a string constant to a
    /// module.
    struct AddStringConstant : boost::proto::callable {
      typedef ptr<Expression<Base> > result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             ptr<Symbol<Constant<Base> > > str);
    };

    /// This is a callable transform to construct a constant symbol.
    template<
      typename ConstantTypeGenerator,
      typename Dummy = boost::proto::callable>
    struct ConstructConstantSymbol : boost::proto::callable {
      typedef ptr<Symbol<Constant<Base>>> result_type;

      template<typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     const Expr &expr) {
        ConstantTypeGenerator typeGen;

        // Constant type
        ptr<const Symbol<Type<TypeBase> > > constantType = 
          LookupAndAddSymbol<Symbol<Type<TypeBase> > >()(
            symtab,
            typeGen(symtab,
                    sizeof(typename boost::proto::result_of::value<Expr>::type) * 8));

        typedef typename ConstantTypeGenerator::value_type BaseType;
        typedef Constant<BaseType> ConstantType;

        result_type result =
          mirv::make<Symbol<ConstantType>>(constantType, boost::proto::value(expr));

	return result;
      }
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
      typedef ptr<Symbol<Constant<Base>>> result_type;

      template<typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     const Expr &expr) {
        detail::StringTypeGen typeGen;

        // Constant type
        ptr<const Symbol<Type<TypeBase> > > constantType = 
          LookupAndAddSymbol<Symbol<Type<TypeBase> > >()(
            symtab, typeGen(symtab, boost::proto::value(expr)));

        typedef Constant<std::string> ConstantType;

        result_type result =
          mirv::make<Symbol<ConstantType>>(constantType,
                                           boost::proto::value(expr));

	return result;
      }
    };

    /// This is a callable transform to construct an address constant
    /// symbol.
    struct ConstructAddressConstantSymbol : boost::proto::callable {
      typedef ptr<Symbol<Constant<Base>>> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
			     ptr<Symbol<Global> > symbol);
    };

    /// This is a callable transform to construct an integer constant
    /// symbol.
    template<
      std::int64_t Value,
      typename Dummy = boost::proto::callable>
    struct ConstructIntegerConstantSymbol : boost::proto::callable {
      typedef ptr<Symbol<Constant<Base>>> result_type;

      result_type operator()(ptr<SymbolTable> symtab) {
        // Constant type
        detail::IntegralTypeGen typeGen;
        ptr<const Symbol<Type<TypeBase> > > constantType = 
          LookupAndAddSymbol<Symbol<Type<TypeBase> > >()(
            symtab,
            typeGen(symtab, 64));

        typedef detail::IntegralTypeGen::value_type BaseType;
        typedef Constant<BaseType> ConstantType;

        result_type result =
          mirv::make<Symbol<ConstantType>>(constantType, Value);

	return result;
      }
    };
  }
}

#endif
