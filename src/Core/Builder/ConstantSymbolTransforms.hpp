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
    /// This is a callable transform to construct a constant symbol.
    /// If the symbol exists at the current module, it is an error.
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
  }
}

#endif
