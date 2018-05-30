#include <mirv/Core/IR/Sequence.hpp>
#include <mirv/Core/IR/Producer.hpp>

namespace mirv {
  Sequence::Sequence(std::string Name, ptr<Producer> P) :
    Control(std::move(Name)), TheProducer(P) {
    TheProducer->setSequenceParent(getHandle(this));
  }
}
