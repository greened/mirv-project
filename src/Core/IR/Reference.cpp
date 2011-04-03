#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/TupleType.hpp>

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <iostream>

namespace mirv {
  Reference<Tuple>::Interface::TypePtr
  Reference<Tuple>::Interface::type(void) const 
  {
    ptr<Symbol<Type<TypeBase> > >::const_type elementType =
      (*this->begin())->type();

    auto index = begin();
    // Skip base expression to get to the first dimension.
    ++index;
    do {
      if (ptr<Symbol<Type<Tuple> > >::const_type tupleType =
          dyn_cast<const Symbol<Type<Tuple> > >(elementType)) {
        elementType = tupleType->elementType(*index);
      }
      else {
        ptr<Symbol<Type<Pointer> > >::const_type pointerType =
          safe_cast<const Symbol<Type<Pointer> > >(elementType);
        // The index is simply a pointer offset, so the type returned
        // is always the same.
        elementType = pointerType->getBaseType();
      }
    } while (++index != end());

    return elementType;
  }

  TuplePointer::Interface::TypePtr
  TuplePointer::Interface::type(void) const 
  {
    ptr<Symbol<Type<TypeBase> > >::const_type elementType =
      (*this->begin())->type();

    auto index = begin();
    // Skip base expression to get to the first dimension.
    ++index;
    do {
      ptr<Symbol<Type<Tuple> > >::const_type tupleType =
        safe_cast<const Symbol<Type<Tuple> > >(elementType);
      elementType = tupleType->elementType(*index);
    } while (++index != end());

    // Return a pointer to this type.
    ptr<Symbol<Module> >::type module = elementType->parent<Symbol<Module> >();

    std::ostringstream pointerName;
    print(pointerName, elementType);
    pointerName << " *";

    Symbol<Module>::ConstTypeIterator p = module->typeFind(pointerName.str());
    if (p != module->typeEnd()) {
      return *p;
    }

    ptr<Symbol<Type<TypeBase> > >::const_type pointerType =
      make<Symbol<Type<Pointer> > >(elementType);
    module->typePushBack(pointerType);
    return pointerType;
  }
}
