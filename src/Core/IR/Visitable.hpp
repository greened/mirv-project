#ifndef mirv_Core_IR_Visitable_hpp
#define mirv_Core_IR_Visitable_hpp

#include <mirv/Core/Filter/NodeVisitor.hpp>

namespace mirv {
  /// Enable a class hierarchy to be visited.  This exists as a
  /// separate capability class so that it may be specialized for
  /// different kinds of nodes.
  template<typename NodeType, typename Visitor>
  class Visitable {
  public:
    virtual void accept(Visitor &V) {
      detail::AcceptImpl<NodeType,
        boost::is_base_of<
          boost::enable_shared_from_this<NodeType>,
          NodeType
          >::value
      > impl;
      impl(safe_cast<NodeType>(this), V);
    }
  };
}

#endif
