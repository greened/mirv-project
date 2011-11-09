#ifndef mirv_Core_IR_TupleTypeFwd_hpp
#define mirv_Core_IR_TupleTypeFwd_hpp

#include <mirv/Core/IR/TypeFwd.hpp>

namespace mirv {
  struct Tuple;

  namespace detail {
    struct TupleInterface;

    /// Define the visitation base type for tuple type symbols.
    template<>
    struct VisitorBaseTypeOfTypeSymbol<Tuple> {
      typedef Symbol<Type<Derived> > VisitorBaseType;
    };
    /// Define the base type of tuple type symbols.
    template<>
    struct BaseTypeOfTypeSymbol<Tuple> {
      typedef TupleInterface BaseType;
    };
  }
}

#endif
