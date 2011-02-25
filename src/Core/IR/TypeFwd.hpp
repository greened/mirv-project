#ifndef mirv_core_ir_type_fwd_hh
#define mirv_core_ir_type_fwd_hh

namespace mirv {
  template<typename Tag>
  class Type;
  class TypeBase;
  class LeafType;
  class InnerType;
  class Simple;
  struct Integral;
  struct Floating;
  struct Derived;
  struct Array;
  struct Pointer;
  struct FunctionType;
  struct StructType;
  struct Tuple;
}

#endif
