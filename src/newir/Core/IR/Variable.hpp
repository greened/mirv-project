#ifndef mirv_Core_IR_Variable_hpp
#define mirv_Core_IR_Variable_hpp

#include <Core/IR/detail/IRNode.hpp>

namespace mirv {
  // Represent a function or routine.
  class Variable : public Symbol<Variable> {
  public:
    enum {
      IRKind = detail::IRNode::Variable;
    };

    Variable(const std::string &name) : Symbol(name) {}
  };
}

#endif
