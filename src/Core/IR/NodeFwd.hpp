#ifndef mirv_Core_IR_NodeFwd_hpp
#define mirv_Core_IR_NodeFwd_hpp

#include <mirv/Core/IR/Base.hpp>

namespace mirv {
  /// This is a base template that can be used as the root of various
  /// class hierarchies.
   template<typename Tag>
   class Node;

  template<typename Child, typename BaseType, bool TrackParent>
  class InnerImpl;

  template<typename Traits>
  class Inner;

  template<typename Traits>
  class LeafImpl;

  namespace detail {
    template<typename NodeType>
    struct VisitorBaseTypeOf;

    template<typename NodeType>
    struct BaseTypeOf;

    template<typename Child, typename BaseType, bool TrackParent>
    struct VisitorBaseTypeOf<InnerImpl<Child, BaseType, TrackParent> > {
      typedef Node<Base> VisitorBaseType;
    };

    template<typename Child, typename BT, bool TrackParent>
    struct BaseTypeOf<InnerImpl<Child, BT, TrackParent> > {
      typedef BT BaseType;
    };

    template<typename Traits>
    class InnerInterface;

    template<typename Traits>
    struct VisitorBaseTypeOf<Node<Inner<Traits> > > {
      typedef typename Traits::VisitorBaseType VisitorBaseType;
    };

    template<typename Traits>
    struct BaseTypeOf<Node<Inner<Traits> > > {
      typedef InnerInterface<Traits> BaseType;
    };

    template<typename Tag>
    struct VisitorBaseTypeOf<LeafImpl<Tag> > {
      typedef Tag VisitorBaseType;
    };

    template<typename Tag>
    struct BaseTypeOf<LeafImpl<Tag> > {
      typedef Tag BaseType;
    };
  }
}

#endif
