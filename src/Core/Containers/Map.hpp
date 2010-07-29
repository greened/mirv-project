#ifndef mirv_Core_Containers_Map_hpp
#define mirv_Core_Containers_Map_hpp

#include <map>

namespace mirv {
  template<typename Key, typename Value>
  struct Map {
    typedef std::map<Key, Value> type;
  };
}

#endif
