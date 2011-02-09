#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/ArrayType.hpp>

namespace mirv {
  Reference<Array>::Interface::TypePtr
  Reference<Array>::Interface::type(void) const 
  {
    // The type is the type of the array with each dimension
    // stripped off.
    ptr<Symbol<Type<Array> > >::const_type arrayType =
      safe_cast<const Symbol<Type<Array> > >((*this->begin())->type());

    if ((this->size() - 1) == arrayType->dimensionSize()) {
      // We completely indexed the array.
      return arrayType->getElementType();
    }

    Symbol<Type<Array> >::ConstDimensionIterator dimStart =
      arrayType->dimensionBegin();

    Symbol<Type<Array> >::ConstDimensionIterator dimEnd = dimStart;
    // Size of this expression includes the base object and we
    // only want to count dimensions.
    std::advance(dimEnd, arrayType->dimensionSize() - (this->size() - 1));

    ptr<Symbol<Module> >::type module =
      arrayType->parent<Symbol<Module> >();

    checkInvariant(module, "Could not get parent module for type");

    Symbol<Module>::TypeIterator arefType = module->
      typeFind(Array::getName(arrayType->getElementType(),
                              dimStart,
                              dimEnd));
        
    if (arefType != module->typeEnd()) {
      return *arefType;
    }

    // Create a new array type and add it to the module.
    TypePtr result =
      mirv::make<Symbol<Type<Array> > >(arrayType->getElementType(),
                                        dimStart,
                                        dimEnd);
    module->typePushBack(result);
    return result;
  }
}
