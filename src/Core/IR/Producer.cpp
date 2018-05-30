#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Producer.hpp>
#include <mirv/Core/IR/ValueProducer.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <iostream>

namespace mirv {
  ptr<Module> Producer::getParentModule(void) const {
    return getControlParent()->getParentFunction()->getParentModule();
  }

  ptr<Function> Producer::getParentFunction(void) const {
    return getControlParent()->getParentFunction();
  }

  ptr<Control> Producer::getControlParentHelper(void) const {
    // Check uses.  If one of them is the right type, return it.
    // Otherwise pick a ValueConsumer use and follow it.
    //
    // This works because either ValueProducers eventually flow to a
    // Store or they flow to a Phi, both of which are anchored at
    // Sequence constructs.  The only cycles in the data flow graph go
    // through Phi nodes so we will eventually hit a control
    // structure.

    if (auto Seq = getSequenceParent()) {
      return Seq;
    }

    // Not anchored by a sequence, so this should have exactly one
    // use.  Anything not a ValueProducer (stores, subroutine calls,
    // etc.) should be anchored by a sequence.
    auto VP = safe_cast<const ValueProducer>(getHandle(this));

    checkInvariant(VP->NumValueUses() == 1, "Multi-use value is not anchored");

    for (auto &D : VP->ValueUses()) {
      auto C = D.getConsumer();
      if (auto P = dyn_cast<Conditional>(C)) {
        // Conditional Control, Sequence, etc.
        return P;
      }
      if (auto Prd = dyn_cast<Producer>(C)) {
        // Walk up Producer parents.
        return Prd->getControlParent();
      }
    }

    // This should not happen.
    unreachable("No control parent for producer");
    return nullptr;
  }

  void Producer::acceptImpl(ValueVisitor &V) {}

  void Producer::dump(void) {
    print(std::cout, getHandle(this));
  }
}
