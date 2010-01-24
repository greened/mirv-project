#include <mirv/core/filter/Dataflow.hpp>
#include <mirv/core/ir/Attribute.hpp>

namespace mirv {
  template AttributeManager<NullDataflow> *
  AttributeManager<NullDataflow>::inst;
  template<> AttributeManager<NullDataflow> *
  AttributeManager<NullDataflow>::inst = 0;
}
