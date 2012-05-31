#ifndef mirv_Core_Containers_Set_hpp
#define mirv_Core_Containers_Set_hpp

#include <set>

namespace mirv {
  template<typename Value>
  struct Set {
    typedef std::set<Value> type;
  };
}

#endif
