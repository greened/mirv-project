#include <mirv/Core/Filter/Dataflow.hpp>
#include <mirv/Core/IR/Attribute.hpp>

namespace mirv {
  template AttributeManager<NullDataflow> *
  AttributeManager<NullDataflow>::inst;
  template<> AttributeManager<NullDataflow> *
  AttributeManager<NullDataflow>::inst = 0;
}
