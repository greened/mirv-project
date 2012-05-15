#ifndef mirv_Core_Builder_Make_hpp
#define mirv_Core_Builder_Make_hpp

#include <mirv/Core/Memory/Heap.hpp>

namespace mirv {
  /// Construct an object with no constructor arguments.
  template<typename Obj>
  typename mirv::ptr<Obj>
  make(void) {
    return Obj::make();
  }

  /// Construct an object with one constructor argument.
  template<typename Obj, typename A1>
  typename mirv::ptr<Obj>
  make(A1 a1) {
    return Obj::make(a1);
  }

  /// Construct an object with two constructor arguments.
  template<typename Obj, typename A1, typename A2>
  typename mirv::ptr<Obj>
  make(A1 a1, A2 a2) {
    return Obj::make(a1, a2);
  }

  /// Construct an object with three constructor arguments.
  template<typename Obj, typename A1, typename A2, typename A3>
  typename mirv::ptr<Obj>
  make(A1 a1, A2 a2, A3 a3) {
    return Obj::make(a1, a2, a3);
  }

  /// Construct an object with four constructor arguments.
  template<typename Obj, typename A1, typename A2, typename A3, typename A4>
  typename mirv::ptr<Obj>
  make(A1 a1, A2 a2, A3 a3, A4 a4) {
    return Obj::make(a1, a2, a3, a4);
  }
}

#endif
