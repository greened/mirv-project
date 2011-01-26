#ifndef mirv_Core_Filter_NodeVisitor_hpp
#define mirv_Core_Filter_NodeVisitor_hpp

namespace mirv {
  namespace detail {
    template<typename NodeType, bool HasSharedFromThis>
    class AcceptImpl {
    public:
      template<typename VisitorType>
      void operator()(NodeType *This, VisitorType &V) {
        typename ptr<NodeType>::type p(This->shared_from_this());
        V.visit(p);
      }

      template<typename VisitorType>
      void operator()(const NodeType *This, VisitorType &V) {
        typename ptr<NodeType>::const_type p(This->shared_from_this());
        V.visit(p);
      }
    };

    template<typename NodeType>
    class AcceptImpl<NodeType, false> {
    public:
      template<typename VisitorType>
      void operator()(NodeType *This, VisitorType &V) {
        error("Wrong accept called");
      }

      template<typename VisitorType>
      void operator()(const NodeType *This, VisitorType &V) {
        error("Wrong accept called");
      }
    };
  }
}

#endif
