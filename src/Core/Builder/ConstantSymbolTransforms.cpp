#include <mirv/Core/Builder/ConstantSymbolTransforms.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/AddressConstant.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/PointerType.hpp>

namespace mirv {
  namespace Builder {
    ptr<Symbol<Constant<Base> > >::type
    ConstructAddressConstantSymbol::operator()(ptr<SymbolTable>::type symtab,
                                               ptr<Symbol<Global> >::type symbol)
    {
      ptr<Symbol<Type<TypeBase> > >::const_type constantType = 
        LookupAndAddSymbol<Symbol<Type<TypeBase> > >()(
          symtab,
          mirv::make<Symbol<Type<Pointer> > >(symbol->type()));

      typedef Constant<Address> ConstantType;

      result_type result =
        mirv::make<Symbol<ConstantType>>(constantType, symbol);

      return result;
    }
  }  
}
