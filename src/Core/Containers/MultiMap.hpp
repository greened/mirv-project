#ifndef mirv_Core_Containers_MultiMap_hpp
#define mirv_Core_Containers_MultiMap_hpp

#include <map>

namespace mirv {
  template<typename Key, typename Value>
  struct MultiMap {
    typedef std::multimap<Key, Value> type;
  };
}

#endif
