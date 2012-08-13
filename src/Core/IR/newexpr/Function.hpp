#ifndef mirv_Core_IR_Function_hpp
#define mirv_Core_IR_Function_hpp

#include <Core/IR/Statement.hpp>

namespace mirv {
  // Represent a function or routine.
  class Function : public Node<Statement> {
  };
}

#endif
