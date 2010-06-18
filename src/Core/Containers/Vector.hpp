#ifndef mirv_Core_Containers_Vector_hpp
#define mirv_Core_Containers_Vector_hpp

#include <vector>

namespace mirv {
  template<typename Item>
  struct Vector {
    typedef std::vector<Item> type;
  };
}

#endif
