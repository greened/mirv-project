#ifndef mirv_core_util_cast_hh
#define mirv_core_util_cast_hh

#include <mirv/util/debug.hh>

#include <boost/type_traits.hpp>

namespace mirv {
  template<typename To, typename From>
  inline To safe_ptr_cast(From val)
  {
    typedef typename boost::add_reference<typename boost::remove_pointer<To>::type>::type ref_type;

    if (DebugManager::Instance().IsActive()
	&& DebugManager::Instance().Feature(DebugManager::SafeCast)) {
      ref_type thisr(dynamic_cast<ref_type>(*val));
      return &thisr;
    }
    else {
      return static_cast<To>(val);
    }
  }
}

#endif
