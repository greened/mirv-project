#ifndef mirv_Core_IR_IntegralTypeFwd_hpp
#define mirv_Core_IR_IntegralTypeFwd_hpp

#include <mirv/Core/IR/TypeFwd.hpp>

namespace mirv {
  /// Integer types.
  struct Integral;

  namespace detail {
    class IntegralInterface;

    /// Define the visitation base type for integer type symbols.
    template<>
    struct VisitorBaseTypeOfTypeSymbol<Integral> {
      typedef Symbol<Type<Simple> > VisitorBaseType;
    };
    /// Define the base type of integer type symbols.
    template<>
    struct BaseTypeOfTypeSymbol<Integral> {
      typedef IntegralInterface BaseType;
    };
  }
}

#endif
