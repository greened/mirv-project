#ifndef mirv_Core_Builder_ConstantSymbolTransforms_hpp
#define mirv_Core_Builder_ConstantSymbolTransforms_hpp

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/ConstantFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      struct IntegralTypeGen {
        typedef ptr<Symbol<Type<TypeBase> > >::const_type result_type;
        typedef std::uint64_t value_type;

        result_type operator()(ptr<SymbolTable>::type symtab,
                               size_t bitsize);
      };
 
      struct FloatingTypeGen {
        typedef ptr<Symbol<Type<TypeBase> > >::const_type result_type;
        typedef double value_type;

        result_type operator()(ptr<SymbolTable>::type symtab,
                               size_t bitsize);
      };
    }

    /// This is a callable transform to construct a constant symbol.
    template<
      typename ConstantTypeGenerator,
      typename Dummy = boost::proto::callable>
    struct ConstructConstantSymbol : boost::proto::callable {
      typedef typename ptr<Symbol<Constant<Base>>>::type result_type;

      template<typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     const Expr &expr) {
        ConstantTypeGenerator typeGen;

        // Constant type
        ptr<Symbol<Type<TypeBase> > >::const_type constantType = 
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

    /// This is a callable transform to construct an address constant
    /// symbol.
    struct ConstructAddressConstantSymbol : boost::proto::callable {
      typedef typename ptr<Symbol<Constant<Base>>>::type result_type;

      result_type operator()(ptr<SymbolTable>::type symtab,
			     ptr<Symbol<Global> >::type symbol);
    };

    /// This is a callable transform to construct an integer constant
    /// symbol.
    template<
      std::int64_t Value,
      typename Dummy = boost::proto::callable>
    struct ConstructIntegerConstantSymbol : boost::proto::callable {
      typedef typename ptr<Symbol<Constant<Base>>>::type result_type;

      result_type operator()(ptr<SymbolTable>::type symtab) {
        // Constant type
        detail::IntegralTypeGen typeGen;
        ptr<Symbol<Type<TypeBase> > >::const_type constantType = 
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
