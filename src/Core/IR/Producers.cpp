#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  Allocate::Allocate(std::string N, TypeHandle T) :
      TheName(std::move(N)),
      TheType(IRBuilder::getPointerType(T)),
      ValType(T) {};

  Load::TypeHandle Load::getTypeImpl(void) const {
    return safe_cast<const PointerType>(getOperand()->getType())->getBaseType();
  }

  Store::TypeHandle Store::getTypeImpl(void) const {
    return getStoredValue()->getType();
  }

  TuplePointer::TypeHandle TuplePointer::getTypeImpl(void) const {
    // std::cerr << "TuplePointer::type expr:\n";
    // print(std::cerr, this->getSharedHandle());
    // std::cerr << "\n";
    ptr<const Type> elementType = nullptr;
    bool SawPointer = false;
    for (auto Index : Operands()) {
      if (elementType == nullptr) {
        // First operand is the base pointer.
        elementType = Index->getType();
        continue;
      }
      // The top-level type could be a pointer.  If there are any
      // pointers down in lower positions there should have been a
      // load to access them.
      if (auto pointerType = dyn_cast<const PointerType>(elementType)) {
        checkInvariant(!SawPointer, "Multiple pointer types in TuplePointer!");
        // The first "index" is an offset from the pointer so the type
        // remains a pointer.
        SawPointer = true;
        continue;
      }
      auto tupleType = safe_cast<const TupleType>(elementType);
      elementType = tupleType->ElementType(Index);
    }
    checkInvariant(elementType != nullptr, "No TuplePointer indices!");

    // Return a pointer to this type.
    return IRBuilder::getPointerType(elementType);
  }
}
