#include <mirv/Core/IR/DataProducer.hpp>

namespace mirv {
  void DataProducer::acceptImpl(ValueVisitor &V) {
    V.visit(*this);
  }

  void DataProducer::addDependence(DataDependence &D) {
    TheDataUses.push_back(D);
  }

  void DataProducer::removeDependence(DataDependence &D) {
    TheDataUses.erase(TheDataUses.iterator_to(D));
    if (NumDataUses() == 0) {
      delete this;
    }
  }
}
