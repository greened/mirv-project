#include <mirv/Core/IR/Dependence.hpp>
#include <mirv/Core/IR/ValueConsumer.hpp>
#include <mirv/Core/IR/ValueProducer.hpp>

namespace mirv {
  ValueDependence::ValueDependence(ProducerHandle P,
                                   ConsumerHandle C,
                                   ConsumerOperandIndex O) :
    TheProducer(P), TheConsumer(C), OperandIndex(O) {
    TheProducer->addDependence(*this);
  }

  ValueDependence::~ValueDependence(void) {
    TheProducer->removeDependence(*this);
  }
}
