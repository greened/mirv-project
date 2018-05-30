#ifndef mirv_Core_IR_NodeType_hpp
#define mirv_Core_IR_NodeType_hpp

#include <mirv/Core/Memory/Heap.hpp>

#include <string>

namespace mirv {
  class Type;

  class NodeType final {
    ptr<const Type> TheType;

  public:
    NodeType(ptr<const Type> T) : TheType(std::move(T)) {}

    ptr<const Type>  getType(void) const {
      return TheType;
    }
  };
}

#endif
