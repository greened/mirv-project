#ifndef mirv_Core_IR_Function_hpp
#define mirv_Core_IR_Function_hpp

#include <Core/IR/Statement.hpp>

#include <Core/IR/detail/IRNode.hpp>

namespace mirv {
  // Represent a function or routine.
  class Function : public Node<Statement> {
  public:
    enum {
      IRKind = detail::IRNode::Function;
    };
  };
}

#endif
