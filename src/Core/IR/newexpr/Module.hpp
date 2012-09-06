#ifndef mirv_Core_IR_Module_hpp 
#define mirv_Core_IR_Module_hpp

#include <Core/IR/Function.hpp>

include <Core/IR/detail/IRNode.hpp>

namespace mirv {
  class Module : public Node<Function> {
  public:
    enum {
      IRKind = detail::IRNode::Module;
    };
  };
}

#endif
