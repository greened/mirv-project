#ifndef mirv_Core_Memory_Heap_hpp
#define mirv_Core_Memory_Heap_hpp

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
    typedef boost::weak_ptr<Type> weak_type;
    typedef boost::weak_ptr<const Type> const_weak_type;
  };

  // TODO: Implement make_shared using variadic template args.
}

#endif
