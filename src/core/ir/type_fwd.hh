#ifndef mirv_core_ir_type_fwd_hh
#define mirv_core_ir_type_fwd_hh

namespace mirv {
  template<typename Tag>
  class Type;
  class TypeBase;
  class LeafType;
  class InnerType;
  class Simple;
  template<int Size>
  struct Integral;
  struct IntegralBase;
  template<int Size>
  struct Floating;
  struct FloatingBase;
  struct Derived;
  struct Array;
  struct Pointer;
}

#endif
