#ifndef mirv_Core_Utility_Cast_hpp
#define mirv_Core_Utility_Cast_hpp

#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <boost/type_traits.hpp>

namespace mirv {
  /// Safely case from pointer to From to pointer to To, taking
  /// downcasts into proper consideration.  This assumes the cast is
  /// legal and aborts if it is not.
  template<typename To, typename From>
  inline To *safe_cast(From *val)
  {
    typedef typename boost::add_reference<typename boost::remove_pointer<To *>::type>::type ref_type;

    ref_type thisr(dynamic_cast<ref_type>(*val));
    return &thisr;
  }

  /// This is a safe_cast overload for boost::shared_ptr.
  template<typename To, typename From>
  inline typename boost::shared_ptr<To> safe_cast(boost::shared_ptr<From> val)
  {
    typename boost::shared_ptr<To> ret = boost::dynamic_pointer_cast<To>(val);
    checkInvariant(ret, "Failed safe_cast");

    return ret;
  }

  // For some reason this doesn't work.
  // template<typename To, typename From>
  // typename ptr<To>::type dyn_cast(typename ptr<From>::type const &val)
  // {
  //   typename ptr<To>::type ret = boost::dynamic_pointer_cast<To>(val);
  //   return ret;
  // }

  /// Implement a dynamic cast from pointer to From to pointer to To.
  /// We need this to handle smart pointers properly.
  template<typename To, typename From>
  typename boost::shared_ptr<To> dyn_cast(typename boost::shared_ptr<From> const &val)
  {
    typename ptr<To>::type ret = boost::dynamic_pointer_cast<To>(val);
    return ret;
  }

  /// Do a non-checking cast from pointer to From to pointer to To.
  /// This assumes the cast is legal.  Performing an illegal cast
  /// results in undefined behavior.  This cast is checked in debug
  /// mode.
  template<typename To, typename From>
  inline To *fast_cast(From *val)
  {
    typedef typename boost::add_reference<typename boost::remove_pointer<To *>::type>::type ref_type;

    if (DebugManager::instance().isActive()
	&& DebugManager::instance().feature(DebugManager::SafeCast)) {
      ref_type thisr(dynamic_cast<ref_type>(*val));
      return &thisr;
    }
    else {
      return static_cast<To>(val);
    }
  }

  /// A fast_cast specialization for boost::shared_ptr.
  template<typename To, typename From>
  typename boost::shared_ptr<To> fast_cast(typename boost::shared_ptr<From> const &val)
  {
    if (DebugManager::instance().isActive()
	&& DebugManager::instance().feature(DebugManager::SafeCast)) {
      typename ptr<To>::type ret = dyn_cast<To>(val);
      return ret;
    }
    else {
      typename ptr<To>::type ret = boost::static_pointer_cast<To>(val);
      return ret;
    }
  }
}

#endif
