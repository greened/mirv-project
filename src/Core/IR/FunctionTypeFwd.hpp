#ifndef mirv_Core_IR_FunctionTypeFwd_hpp
#define mirv_Core_IR_FunctionTypeFwd_hpp

#include <mirv/Core/IR/TypeFwd.hpp>

namespace mirv {
  /// Name some booleans to clarify code when setting a function
  /// vararg.
  enum VarargMark {
    Vararg = true,
    NotVararg = false
  };

  /// A function type.  Function types have a return type and a list
  /// of parameter types.  A function that does not return anything
  /// will have a pointer to zero as its return type.  A funtion that
  /// does not take any arguments will have an empty argument
  /// sequence.
  struct FunctionType;

  namespace detail {
    struct FunctionTypeInterface;

    /// Define the visitation base type for function type symbols.
    template<>
    struct VisitorBaseTypeOfTypeSymbol<FunctionType> {
      typedef Symbol<Type<Derived> > VisitorBaseType;
    };
    /// Define the base type of tuple type symbols.
    template<>
    struct BaseTypeOfTypeSymbol<FunctionType> {
      typedef FunctionTypeInterface BaseType;
    };
  }
}

#endif
