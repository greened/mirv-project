#ifndef mirv_Core_IR_Visitable_hpp
#define mirv_Core_IR_Visitable_hpp

#include <mirv/Core/Utility/Debug.hpp>

namespace mirv {
  namespace detail {
    /// Return the visitation base type for a given symbol, expression
    /// or statement tag.
    template<typename Op>
    struct VisitorBase {
      typedef typename Op::VisitorBaseType VisitorBaseType;
    };

    /// This is a metafunction to compute the base type of a symbol,
    /// expression or statement.  It relies on a nested member
    /// ::BaseType to specify the base type for the metafunction
    /// argument.
    template<typename Op>
    struct BaseTypeOf {
      typedef typename Op::BaseType BaseType;
    };
  }

  /// Enable a class hierarchy to be visited.  This exists as a
  /// separate capability class so that it may be specialized for
  /// different kinds of nodes.
  template<
    typename NodeType,
    typename Visitor
    >
  class Visitable : public detail::BaseTypeOf<NodeType>::BaseType {
  private:
    typedef typename detail::BaseTypeOf<NodeType>::BaseType BaseType;

  public:
    Visitable(void) : BaseType() {}
    template<typename ...Arg>
    Visitable(const Arg &...arg) : BaseType(arg...) {};

    // The visitor classes overloads based on ptr<...>::type.  This
    // requires the smart pointer to determine whether one is
    // convertable to another, which in turn requires full definition
    // of the classes pointed to.  This creates a lot of coupling
    // between visited types.  Thus we leave this undefined here and
    // explicitly instantiate it in the various IR source files.  This
    // breaks the coupling and avoids lots of spurious include file
    // dependencies.
    virtual void accept(Visitor &V);
  };

  /// This enables visitation of a class by a const visitor.  It
  /// inherits from Visitable to avoid some MI issues.
  // TODO: Fix MI issues.
  template<
    typename NodeType,
    typename ConstVisitor,
    typename Visitor
    >
  class ConstVisitable : public Visitable<NodeType, Visitor> {
  private:
    typedef Visitable<NodeType, Visitor> BaseType;

  public:
    ConstVisitable(void) : BaseType() {}
    template<typename ...Arg>
    ConstVisitable(const Arg &...arg)
    : BaseType(arg...) {};

    using Visitable<NodeType, Visitor>::accept;

    // The visitor classes overloads based on ptr<...>::type.  This
    // requires the smart pointer to determine whether one is
    // convertable to another, which in turn requires full definition
    // of the classes pointed to.  This creates a lot of coupling
    // between visited types.  Thus we leave this undefined here and
    // explicitly instantiate it in the various IR source files.  This
    // breaks the coupling and avoids lots of spurious include file
    // dependencies.
    virtual void accept(ConstVisitor &V) const;
  };
}

#endif
