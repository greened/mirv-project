#ifndef mirv_Library_MultiMap_hpp
#define mirv_Library_MultiMap_hpp

#include <map>

namespace mirv {
  template<typename K, typename T>
  using MultiMap = std::multimap<K, T>;
}

#endif
