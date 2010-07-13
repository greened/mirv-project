#include <mirv/Core/Filter/Dataflow.hpp>
#include <mirv/Core/IR/Attribute.hpp>

namespace mirv {
  template NodeAttributeManager<NullDataflow> *
  NodeAttributeManager<NullDataflow>::inst;
  template<> NodeAttributeManager<NullDataflow> *
  NodeAttributeManager<NullDataflow>::inst = 0;
}
