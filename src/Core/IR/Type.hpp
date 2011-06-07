#ifndef mirv_Core_IR_Type_hpp
#define mirv_Core_IR_Type_hpp

#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/Symbol.hpp>

namespace mirv {
  struct SymbolVisitor;
  struct Placeholder;

  /// This is the type implementation for all type symbols.  It is a
  /// templated Symbol tag.  Each type is an instance of this template
  /// (Symbol<Type<Integral> >, Symbol<Type<Function> >, etc.).  It
  /// keeps all of the property and visitor logic in one place, hiding
  /// the gory details from the type tags and specific type
  /// interfaces.
  template<typename Tag>
  class Type {
  public:
    typedef typename Tag::BaseType BaseType;
    typedef typename Tag::VisitorBaseType VisitorBaseType;

    Type() {}

    template<typename Arg>
    static std::string getName(Arg &a) {
      return Tag::getName(a);
    }

    template<typename Arg1, typename Arg2>
    static std::string getName(Arg1 &a1, Arg2 &a2) {
      return Tag::getName(a1, a2);
    }

    static void
    initialize(typename ptr<Symbol<Type<Tag> > >::const_type type) {}

    template<typename Arg1, typename Arg2, typename Arg3>
    static std::string getName(Arg1 &a1, Arg2 &a2, Arg3 &a3) {
      return Tag::getName(a1, a2, a3);
    }
  };

  /// A type tag for the base type of all types.
  class TypeBase {
  private:
    typedef Symbol<Base> InterfaceBaseType;

    class Interface : public InterfaceBaseType {
    public:
      typedef ptr<Expression<Base> >::type BitSizeType;
      virtual BitSizeType bitsize(void) const = 0;
      virtual void
      resolve(ptr<Symbol<Type<Placeholder> > >::const_type placeholder,
              ptr<Symbol<Type<TypeBase> > >::const_type replacement) {}
    };

  public:
    typedef Interface BaseType;
    typedef Symbol<Base> VisitorBaseType;
  };

  //  template<>
  //class Symbol<Type<TypeBase> > : public Type<TypeBase>::BaseType {};

  /// A type with no children.
  class LeafType : public LeafImpl<Symbol<Type<TypeBase> > > {
  public:
    typedef LeafImpl<Symbol<Type<TypeBase> > > BaseType;
    typedef Symbol<Type<TypeBase> > VisitorBaseType;
  };

  namespace detail {
    /// A traits class to define various properties of inner types
    /// such as child type, iterator types and other things.
    class InnerTypeTraits {
    public:
      typedef const Symbol<Type<TypeBase> > Child;
      typedef const Symbol<Type<TypeBase> > BaseType;

    private:
      typedef boost::shared_ptr<Child> ChildPtr;
      typedef std::list<ChildPtr> ChildList;

    public:
      /// Make this compatible with certain standard algorithms.
      typedef ChildPtr value_type;
      typedef const ChildPtr & const_reference;

      typedef ChildList::iterator iterator;
      typedef ChildList::reverse_iterator reverse_iterator;
      typedef ChildList::const_iterator const_iterator;
      typedef ChildList::const_reverse_iterator const_reverse_iterator;

      typedef ChildList::size_type size_type;
    };
  }

  /// This is an inner type abstract interface.  It exists because we
  /// need to be able to inherit virtually from inner types (to allow
  /// property symbol visitors to manipulate children) but we do not
  /// want to force subclasses to explicitly initialize the inner type
  /// object.  Separating the interface from the implementation solves
  /// that problem.
  template<>
  class Symbol<Type<Inner<detail::InnerTypeTraits> > >
      : public Inner<detail::InnerTypeTraits>::BaseType {
  private:
    typedef Inner<detail::InnerTypeTraits>::BaseType BaseType;

  public:
    typedef Symbol<Type<TypeBase> > VisitorBaseType;
  };

  /// Define the base class for types with children.
  class InnerTypeBase : public Symbol<Type<Inner<detail::InnerTypeTraits> > > {
  private:
    typedef Symbol<Type<Inner<detail::InnerTypeTraits> > > BaseType;
  };

  /// This is the implementation of inner types.  It is inherited from
  /// once in the hierarchy for any inner types.  This holds the child
  /// pointers and other data necessary for inner types.
  class InnerType : public InnerImpl<
    const Symbol<Type<TypeBase> >,
    InnerTypeBase,
    // TODO: Avoid TrackParent use.
    false> {
  private:
    typedef InnerImpl<
    const Symbol<Type<TypeBase> >,
    InnerTypeBase,
    false> BaseType;
  };

  /// A type with no children that has a specific bit size, for
  /// example integer and floating point types.
  class Simple {
  private:
    typedef LeafType InterfaceBaseType;

    class Interface : public InterfaceBaseType {
    private:
      std::uint64_t bsize;

    public:
      Interface(std::uint64_t s) : bsize(s) {};

      std::uint64_t integerBitSize(void) const {
        return bsize;
      }

      BitSizeType bitsize(void) const;
    };

  public:
    typedef Interface BaseType;
    typedef LeafType VisitorBaseType;
  };

  /// A type that is built upon other types.  For example structures
  /// and pointers.
  struct Derived {
  private:
    typedef InnerType InterfaceBaseType;

    class Interface : public InterfaceBaseType {
    public:
      void resolve(ptr<Symbol<Type<Placeholder> > >::const_type placeholder,
                   ptr<Symbol<Type<TypeBase> > >::const_type replacement);
    };

  public:
    typedef Interface BaseType;
    typedef InnerType VisitorBaseType;
  };
}

#endif
