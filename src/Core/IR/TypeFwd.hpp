#ifndef mirv_core_ir_type_fwd_hh
#define mirv_core_ir_type_fwd_hh

#include <mirv/Core/IR/SymbolFwd.hpp>

namespace mirv {
  template<typename Tag>
  class Type;
  class TypeBase;
  class LeafType;
  class InnerTypeBase;
  class InnerType;
  class Simple;
  struct Derived;

  namespace detail {
    /// Define the vitiation base type for a type symbol.
    template<typename Tag>
    struct VisitorBaseTypeOfTypeSymbol {};

    template<typename Tag>
    struct VisitorBaseTypeOfSymbol<Type<Tag> > {
      typedef typename VisitorBaseTypeOfTypeSymbol<Tag>::VisitorBaseType VisitorBaseType;
    };

    template<typename Tag>
    struct VisitorBaseTypeOf<Symbol<Type<Tag> > > {
      typedef typename VisitorBaseTypeOfSymbol<Type<Tag> >::VisitorBaseType VisitorBaseType;
    };

    /// Define the base type of a general type symbol.
    template<typename Tag>
    struct BaseTypeOfTypeSymbol {};

    template<typename Tag>
    struct BaseTypeOfSymbol<Type<Tag> > {
      typedef typename BaseTypeOfTypeSymbol<Tag>::BaseType BaseType;
    };

    template<typename Tag>
    struct BaseTypeOf<Symbol<Type<Tag> > > {
      typedef typename BaseTypeOfSymbol<Type<Tag> >::BaseType BaseType;
    };

    class TypeBaseInterface;

    /// Define the visitation base type for base type symbols.
    template<>
    struct VisitorBaseTypeOfTypeSymbol<TypeBase> {
      typedef Symbol<Base> VisitorBaseType;
    };
    /// Define the base type of base type symbols.
    template<>
    struct BaseTypeOfTypeSymbol<TypeBase> {
      typedef TypeBaseInterface BaseType;
    };

    /// Define the visitation base type for leaf type symbols.
    template<>
    struct VisitorBaseTypeOfSymbol<LeafType> {
      typedef Symbol<Type<TypeBase> > VisitorBaseType;
    };
    /// Define the base type of leaf type symbols.
    template<>
    struct BaseTypeOfSymbol<LeafType> {
      typedef Symbol<Type<TypeBase> > BaseType;
    };

    class InnerTypeTraits;

    /// Define the visitation base type for inner type symbols.
    template<>
    struct VisitorBaseTypeOfTypeSymbol<Inner<InnerTypeTraits> > {
      typedef Symbol<Type<TypeBase> > VisitorBaseType;
    };
    /// Define the base type of inner type symbols.
    template<>
    struct BaseTypeOfTypeSymbol<Inner<InnerTypeTraits> > {
      typedef InnerInterface<InnerTypeTraits>  BaseType;
    };

    /// Define the visitation base type for inner type symbols.
    template<>
    struct VisitorBaseTypeOf<InnerTypeBase> {
      typedef Symbol<Type<TypeBase> > VisitorBaseType;
    };
    /// Define the base type of inner type symbols.
    template<>
    struct BaseTypeOf<InnerTypeBase> {
      typedef Symbol<Type<Inner<InnerTypeTraits> > > BaseType;
    };

    /// Define the visitation base type for inner type symbols.
    template<>
    struct VisitorBaseTypeOfSymbol<InnerType> {
      typedef Symbol<Type<TypeBase> > VisitorBaseType;
    };
    /// Define the base type of inner type symbols.
    template<>
    struct BaseTypeOfSymbol<InnerType> {
      typedef InnerImpl<
        const Symbol<Type<TypeBase> >,
        InnerTypeBase,
        // TODO: Avoid TrackParent use.
        false> BaseType;
    };

    class SimpleInterface;

    /// Define the visitation base type for simple type symbols.
    template<>
    struct VisitorBaseTypeOfTypeSymbol<Simple> {
      typedef LeafType VisitorBaseType;
    };
    /// Define the base type of inner type symbols.
    template<>
    struct BaseTypeOfTypeSymbol<Simple> {
      typedef SimpleInterface BaseType;
    };

    class DerivedInterface;

    /// Define the visitation base type for simple type symbols.
    template<>
    struct VisitorBaseTypeOfTypeSymbol<Derived> {
      typedef InnerType VisitorBaseType;
    };
    /// Define the base type of inner type symbols.
    template<>
    struct BaseTypeOfTypeSymbol<Derived> {
      typedef DerivedInterface BaseType;
    };
  }
}

#endif
