#ifndef mirv_Core_Memory_Heap_hpp
#define mirv_Core_Memory_Heap_hpp

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>

namespace mirv {
  template<typename Type> using ptr = boost::shared_ptr<Type>;
  template<typename Type> using weak_ptr = boost::weak_ptr<Type>;
}

#endif
