#ifndef mirv_Core_Containers_Vector_hpp
#define mirv_Core_Containers_Vector_hpp

#include <vector>

namespace mirv {
  /// This is a metafunction to compute a general vector type suitable
  /// for use in many situations.
  template<typename Item>
  struct Vector {
    typedef std::vector<Item> type;
  };
}

#endif
