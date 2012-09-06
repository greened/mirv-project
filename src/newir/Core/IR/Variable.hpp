#ifndef mirv_Core_IR_Variable_hp
#define mirv_Core_IR_Variable_hpp

#include <Core/IR/detail/IRNode.hpp>

namespace mirv {
  // Represent a function or routine.
  class Variable {
  public:
    enum {
      IRKind = detail::IRNode::Variable;
    };
  };
}

#endif
