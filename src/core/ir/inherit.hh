#ifndef mirv_core_ir_inherit_hh
#define mirv_core_ir_inherit_hh

// Define inheritance mechanisms for extending the class hierarchy.

#include <mirv/ir/base.hh>

namespace mirv {
  // Inherit virtually
  template<typename Base>
  struct Virtual : public virtual Base {};
}

#endif
