#ifndef mirv_core_mem_heap_hh
#define mirv_core_mem_heap_hh

#include <boost/shared_ptr.hpp>

namespace mirv {
  template<typename Type>
  struct ptr {
    typedef boost::shared_ptr<Type> type;
    typedef boost::shared_ptr<const Type> const_type;
  };
}

#endif
