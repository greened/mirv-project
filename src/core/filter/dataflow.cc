#include <mirv/core/filter/dataflow.hh>
#include <mirv/core/ir/attribute.hh>

namespace mirv {
  template AttributeManager<NullDataflow> *
  AttributeManager<NullDataflow>::instance;
  template<> AttributeManager<NullDataflow> *
  AttributeManager<NullDataflow>::instance = 0;
}
