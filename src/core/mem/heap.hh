#ifndef MIRVHeap_hh
#define MIRVHeap_hh

#include <boost/shared_ptr.hpp>

namespace MIRV {
  template<typename Type>
  struct ptr {
    typedef boost::shared_ptr<Type> type;
    typedef boost::shared_ptr<const Type> const_type;
  };
}

#endif
