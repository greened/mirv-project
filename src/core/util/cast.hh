#ifndef mirv_core_util_cast_hh
#define mirv_core_util_cast_hh

#include <mirv/core/util/debug.hh>
#include <mirv/core/mem/heap.hh>

#include <boost/type_traits.hpp>

namespace mirv {
  template<typename To, typename From>
  inline To *safe_cast(From *val)
  {
    typedef typename boost::add_reference<typename boost::remove_pointer<To *>::type>::type ref_type;

    ref_type thisr(dynamic_cast<ref_type>(*val));
    return &thisr;
  }

  template<typename To, typename From>
  inline typename ptr<To>::type safe_cast(const typename ptr<From>::type &val)
  {
    typename ptr<To>::type ret = boost::dynamic_pointer_cast<To>(val);
    check_invariant(ret, "Failed safe_cast");

    return ret;
  }

  // For some reason this doesn't work.
  // template<typename To, typename From>
  // typename ptr<To>::type dyn_cast(typename ptr<From>::type const &val)
  // {
  //   typename ptr<To>::type ret = boost::dynamic_pointer_cast<To>(val);
  //   return ret;
  // }

  template<typename To, typename From>
  typename boost::shared_ptr<To> dyn_cast(typename boost::shared_ptr<From> const &val)
  {
    typename ptr<To>::type ret = boost::dynamic_pointer_cast<To>(val);
    return ret;
  }

  template<typename To, typename From>
  inline To *fast_cast(From *val)
  {
    typedef typename boost::add_reference<typename boost::remove_pointer<To *>::type>::type ref_type;

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
