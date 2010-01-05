#ifndef mirv_core_ir_type_fwd_hh
#define mirv_core_ir_type_fwd_hh

namespace mirv {
  template<typename Tag>
  class Type;
  class TypeBase;
  class Simple;
  template<int Size>
  struct Integral;
  struct Floating;
  struct Derived;
  struct Array;
  struct Pointer;
}

#endif
