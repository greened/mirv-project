#include <mirv/Core/IR/ValueProducer.hpp>

namespace mirv {
  void ValueProducer::addDependence(ValueDependence &D) {
    TheValueUses.push_back(D);
  }

  void ValueProducer::removeDependence(ValueDependence &D) {
    TheValueUses.erase(TheValueUses.iterator_to(D));
    // if (NumValueUses() == 0) {
    //   delete this;
    // }
  }
}
