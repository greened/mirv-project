#ifndef mirv_core_builder_make_hh
#define mirv_core_builder_make_hh

#include <mirv/core/mem/heap.hh>

namespace mirv {
  template<typename Obj>
  typename mirv::ptr<Obj>::type
  make(void) {
    return Obj::make();
  }

  template<typename Obj, typename A1>
  typename mirv::ptr<Obj>::type
  make(A1 a1) {
    return Obj::make(a1);
  }

  template<typename Obj, typename A1, typename A2>
  typename mirv::ptr<Obj>::type
  make(A1 a1, A2 a2) {
    return Obj::make(a1, a2);
  }

  template<typename Obj, typename A1, typename A2, typename A3>
  typename mirv::ptr<Obj>::type
  make(A1 a1, A2 a2, A3 a3) {
    return Obj::make(a1, a2, a3);
  }
}

#endif
