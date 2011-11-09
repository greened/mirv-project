#ifndef mirv_core_ir_symbol_fwd_hh
#define mirv_core_ir_symbol_fwd_hh

#include <mirv/Core/IR/NodeFwd.hpp>
#include <mirv/Core/IR/Inherit.hpp>

namespace mirv {
  template<typename Tag>
  class Symbol;
  class InnerSymbol;
  class LeafSymbol;
  class Typed;
  class Named;
  class Global;

  namespace detail {
    /// Define the vitiation base type for a symbol.
    template<typename Tag>
    struct VisitorBaseTypeOfSymbol {};

    template<typename Tag>
    struct VisitorBaseTypeOf<Symbol<Tag> > {
      typedef typename VisitorBaseTypeOfSymbol<Tag>::VisitorBaseType VisitorBaseType;
    };

    /// Define the base type of a general Symbol.
    template<typename Tag>
    struct BaseTypeOfSymbol {};

    template<typename Tag>
    struct BaseTypeOf<Symbol<Tag> > {
      typedef typename BaseTypeOfSymbol<Tag>::BaseType BaseType;
    };

    /// Define the visitation base type for base symbols.
    template<>
    struct VisitorBaseTypeOfSymbol<Base> {
      typedef Node<Base> VisitorBaseType;
    };
    /// Define the base type of base symbols.
    template<>
    struct BaseTypeOfSymbol<Base> {
      typedef Node<Base> BaseType;
    };

    class InnerSymbolTraits;

    /// Define the visitation base type for inner symbols.
    template<>
    struct VisitorBaseTypeOfSymbol<Inner<InnerSymbolTraits> > {
      typedef Symbol<Base> VisitorBaseType;
    };
    /// Define the base type of inner symbols.
    template<>
    struct BaseTypeOfSymbol<Inner<InnerSymbolTraits> > {
      typedef InnerInterface<InnerSymbolTraits> BaseType;
    };

    /// Define the visitation base type for inner symbols.
    template<>
    struct VisitorBaseTypeOf<InnerSymbol> {
      typedef Symbol<Base> VisitorBaseType;
    };
    /// Define the base type of inner symbols.
    template<>
    struct BaseTypeOf<InnerSymbol> {
      typedef InnerInterface<InnerSymbolTraits> BaseType;
    };

    /// Define the visitation base type for leaf symbols.
    template<>
    struct VisitorBaseTypeOf<LeafSymbol> {
      typedef Symbol<Base> VisitorBaseType;
    };
    /// Define the base type of leaf symbols.
    template<>
    struct BaseTypeOf<LeafSymbol> {
      typedef LeafImpl<Virtual<Symbol<Base> > > BaseType;
    };

    class TypedInterface;

    /// Define the visitation base type for typed symbols.
    template<>
    struct VisitorBaseTypeOfSymbol<Typed> {
      typedef Symbol<Base> VisitorBaseType;
    };
    /// Define the base type of typed symbols.
    template<>
    struct BaseTypeOfSymbol<Typed> {
      typedef TypedInterface BaseType;
    };

    class NamedInterface;

    /// Define the visitation base type for named symbols.
    template<>
    struct VisitorBaseTypeOfSymbol<Named> {
      typedef Symbol<Base> VisitorBaseType;
    };
    /// Define the base type of named symbols.
    template<>
    struct BaseTypeOfSymbol<Named> {
      typedef NamedInterface BaseType;
    };

    class GlobalInterface;

    /// Define the visitation base type for global symbols.
    template<>
    struct VisitorBaseTypeOfSymbol<Global> {
      typedef Symbol<Typed> VisitorBaseType;
    };
    /// Define the base type of global symbols.
    template<>
    struct BaseTypeOfSymbol<Global> {
      typedef GlobalInterface BaseType;
    };
  }
}

#endif
