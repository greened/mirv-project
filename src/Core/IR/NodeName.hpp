#ifndef mirv_Core_IR_NodeName_hpp
#define mirv_Core_IR_NodeName_hpp

#include <string>

namespace mirv {
  class NodeName final {
    std::string TheName;

  public:
    NodeName(std::string N) : TheName(std::move(N)) {}

    const std::string &getName(void) const {
      return TheName;
    }
  };
}

#endif
