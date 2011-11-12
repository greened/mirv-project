#ifndef mirv_Core_IR_FloatingTypeFwd_hpp
#define mirv_Core_IR_FloatingTypeFwd_hpp

#include <mirv/Core/IR/TypeFwd.hpp>

namespace mirv {
  /// Floating point types.
  struct Floating;

  namespace detail {
    class FloatingInterface;

    /// Define the visitation base type for float type symbols.
    template<>
    struct VisitorBaseTypeOfTypeSymbol<Floating> {
      typedef Symbol<Type<Simple> > VisitorBaseType;
    };
    /// Define the base type of float type symbols.
    template<>
    struct BaseTypeOfTypeSymbol<Floating> {
      typedef FloatingInterface BaseType;
    };
  }
}

#endif
