#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/AddressConstant.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>

namespace mirv {
  namespace Builder {
    ptr<Expression<Base> >::type
    ConstructGlobalReference::operator()(boost::shared_ptr<SymbolTable> symtab,
                                         ptr<Symbol<GlobalVariable> >::type global)
    {
      auto inttype = LookupAndAddSymbol<Symbol<Type<TypeBase> > >()(
        symtab,
        Symbol<Type<Integral> >::make(64));

      auto ptrtype = LookupAndAddSymbol<Symbol<Type<TypeBase> > >()(
        symtab,
        Symbol<Type<Pointer> >::make(global->type()));

      return make<Expression<Load> >(
        make<Expression<Reference<Constant<Base> > > >(
          make<Symbol<Constant<Address> > >(ptrtype, global)));
    }
  }
}
