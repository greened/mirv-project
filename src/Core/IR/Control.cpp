#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <iostream>

namespace mirv {
  void Control::acceptImpl(ControlVisitor &V) {
    V.visit(*this);
  }

  ptr<Module> Control::getParentModule(void) const {
    if (ParentFunction) {
      return ParentFunction->getParentModule();
    }
    if (auto P = getControlParent<Block>()) {
      return P->getParentFunction()->getParentModule();
    }
    return nullptr;
  }

  ptr<Function> Control::getParentFunction(void) const {
    if (ParentFunction) {
      return ParentFunction;
    }
    if (auto P = getControlParent<Block>()) {
      return P->getParentFunction();
    }
    return nullptr;
  }

  void Control::dump(void) {
    print(std::cout, getHandle(this));
  }
}
