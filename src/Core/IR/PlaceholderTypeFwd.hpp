#ifndef mirv_Core_IR_PlaceholderTypeFwd_hpp
#define mirv_Core_IR_PlaceholderTypeFwd_hpp

namespace mirv {
  /// A pointer type.
  struct Placeholder;

  namespace detail {
    struct PlaceholderInterface;

    /// Define the visitation base type for pointer type symbols.
    template<>
    struct VisitorBaseTypeOfTypeSymbol<Placeholder> {
      typedef Symbol<Type<TypeBase> > VisitorBaseType;
    };
    /// Define the base type of pointer type symbols.
    template<>
    struct BaseTypeOfTypeSymbol<Placeholder> {
      typedef PlaceholderInterface BaseType;
    };
  }
}

#endif
