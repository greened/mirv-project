#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/TupleType.hpp>

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
      ptr<Symbol<Type<Tuple> > >::const_type tupleType =
        safe_cast<const Symbol<Type<Tuple> > >(elementType);
      elementType = tupleType->elementType(*index);
    } while (++index != end());

    return elementType;
  }
}
