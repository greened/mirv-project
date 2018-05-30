#ifndef mirv_Library_Map_hpp
#define mirv_Library_Map_hpp

#include <map>

namespace mirv {
  template<typename K, typename T>
  using Map = std::map<K, T>;
}

#endif
