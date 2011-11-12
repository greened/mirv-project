#ifndef mirv_Core_IR_PointerTypeFwd_hpp
#define mirv_Core_IR_PointerTypeFwd_hpp

#include <mirv/Core/IR/TypeFwd.hpp>

namespace mirv {
  /// A pointer type.
  struct Pointer;

  namespace detail {
    struct PointerInterface;

    /// Define the visitation base type for pointer type symbols.
    template<>
    struct VisitorBaseTypeOfTypeSymbol<Pointer> {
      typedef Symbol<Type<Derived> > VisitorBaseType;
    };
    /// Define the base type of pointer type symbols.
    template<>
    struct BaseTypeOfTypeSymbol<Pointer> {
      typedef PointerInterface BaseType;
    };
  }
}

#endif
