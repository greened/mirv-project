#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/IR/TupleType.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Reference.hpp>

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <iostream>

namespace mirv {
  detail::LoadInterface::LoadInterface(ChildPtr address)
      : Expression<Unary>(address) 
  {
    checkInvariant(dyn_cast<const Symbol<Type<Pointer> > >(address->type()),
      "Load source must have pointer type");
  }
  
  detail::LoadInterface::TypePtr
  detail::LoadInterface::type(void) const 
  {
    ptr<const Symbol<Type<TypeBase> > > ptrType =
      (*this->begin())->type();

    ptr<const Symbol<Type<Pointer> > > pointerType =
      safe_cast<const Symbol<Type<Pointer> > >((*this->begin())->type());

    return pointerType->getBaseType();
  }

  Expression<Base> *
  detail::TuplePointerInterface::cloneImpl(void) {
    std::vector<ptr<Expression<Base> >> children;

    for (auto i = begin(); i != end(); ++i) {
      children.push_back((*i)->clone());
    }

    ptr<Expression<TuplePointer> > expr(
      mirv::make<Expression<TuplePointer> >(*children.begin(),
                                            children.begin() + 1,
                                            children.end()));
    Expression<TuplePointer> *result = expr.get();
    expr.reset();
    return result;
  }

  detail::TuplePointerInterface::TypePtr
  detail::TuplePointerInterface::type(void) const 
  {
    // std::cerr << "TuplePointer::type expr:\n";
    // print(std::cerr, this->getSharedHandle());
    // std::cerr << "\n";

    ptr<const Symbol<Type<TypeBase> > > elementType =
      (*this->begin())->type();

    // std::cerr << "TuplePointer::type element:\n";
    // print(std::cerr, elementType);
    // std::cerr << "\n";

    auto index = begin();
    // Skip base expression to get to the first dimension.
    ++index;

    // The top-level type could be a pointer.  If there are any
    // pointers down in lower positions there should have been a load
    // to access them.
    if (ptr<const Symbol<Type<Pointer> > > pointerType =
        dyn_cast<const Symbol<Type<Pointer> > >(elementType)) {
      elementType = pointerType->getBaseType();
      // std::cerr << "TuplePointer::type element:\n";
      // print(std::cerr, elementType);
      // std::cerr << "\n";
      // Account for the pointer index.
      checkInvariant(index != end(), "Not enough operands for TuplePointer");
      ++index;
    }

    while (index != end()) {
      ptr<const Symbol<Type<Tuple> > > tupleType =
        safe_cast<const Symbol<Type<Tuple> > >(elementType);

      elementType = tupleType->elementType(*index);
      // std::cerr << "TuplePointer::type element:\n";
      // print(std::cerr, elementType);
      // std::cerr << "\n";

      ++index;
    }

    // Return a pointer to this type.
    ptr<Symbol<Module> > module = elementType->parent<Symbol<Module> >();

    std::ostringstream pointerName;
    print(pointerName, elementType);
    pointerName << " *";

    Symbol<Module>::ConstTypeIterator p = module->typeFind(pointerName.str());
    if (p != module->typeEnd()) {
      return *p;
    }

    ptr<const Symbol<Type<TypeBase> > > pointerType =
      make<Symbol<Type<Pointer> > >(elementType);
    module->typePushBack(pointerType);
    return pointerType;
  }

  void detail::LoadInterface::doValidation(void) const {
    if (!empty() && getOperand()) {
      ptr<const Symbol<Type<Pointer> > > pointerType
        = safe_cast<const Symbol<Type<Pointer> > >(getOperand()->type());
      if (this->type() != pointerType->getBaseType()) {
        std::cerr << "Offending expression:\n";
        printWithTypes(std::cerr, this->getSharedHandle());
      }
      checkInvariant(this->type() == pointerType->getBaseType(),
                     "Load type does not match pointer base type");
    }
  }
}
