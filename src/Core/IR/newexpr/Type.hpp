#ifndef mirv_Core_IR_Type_hpp
#define mirv_Core_IR_Type_hpp

#include <Core/IR/Node.hpp>

namespace mirv {
  class Type : public Node<Type> {
  public:
    enum Kind {
      Null,
      Integer,
      Floating,
    };
  };
}

#endif
