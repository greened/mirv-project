#ifndef mirv_core_mem_heap_hh
#define mirv_core_mem_heap_hh

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace mirv {
  /// This is a type generate for smart pointers.  All objects are
  /// referenced through a smart pointer.  This generator serves as a
  /// kind of typedef for the smart pointer type.  When C++0x template
  /// aliases are available, we will use those to specify the smart
  /// pointer type.
  template<typename Type>
  struct ptr {
    typedef boost::shared_ptr<Type> type;
    typedef boost::shared_ptr<const Type> const_type;
  };

  // TODO: Implement make_shared using variadic template args.
}

#endif
