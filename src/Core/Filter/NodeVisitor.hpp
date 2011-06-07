#ifndef mirv_Core_Filter_NodeVisitor_hpp
#define mirv_Core_Filter_NodeVisitor_hpp

namespace mirv {
  namespace detail {
    /// Implement the accept operation for IR nodes.
    template<typename NodeType, bool HasSharedFromThis>
    class AcceptImpl {
    public:
      template<typename VisitorType>
      void operator()(NodeType *This, VisitorType &V) {
        typename ptr<NodeType>::type p(This->shared_from_this());
        V.visit(p);
      }
    };

    /// This is an error-path implementation for types without a
    /// shared handle.  These types are not visitable so it is an
    /// error to define and call an accept method on them.
    template<typename NodeType>
    class AcceptImpl<NodeType, false> {
    public:
      template<typename VisitorType>
      void operator()(NodeType *This, VisitorType &V) {
        error("Wrong accept called");
      }
    };
  }
}

#endif
