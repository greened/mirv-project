// -*-C++-*-
#ifndef mirv_Core_IR_Visitable_ipp
#define mirv_Core_IR_Visitable_ipp

#include <mirv/Core/IR/Visitable.hpp>
#include <mirv/Core/Filter/NodeVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
  template<typename NodeType, typename Visitor>
  void Visitable<NodeType, Visitor>::accept(Visitor&V)
  {
    detail::AcceptImpl<
      NodeType,
      boost::is_base_of<
        boost::enable_shared_from_this<NodeType>,
        NodeType
        >::value
      > impl;
    impl(safe_cast<NodeType>(this), V);
  }

  template<
    typename NodeType,
    typename ConstVisitor,
    typename Visitor
    >
  void
  ConstVisitable<NodeType, ConstVisitor, Visitor>::accept(ConstVisitor&V) const
  {
    detail::AcceptImpl<
      const NodeType,
      boost::is_base_of<
        boost::enable_shared_from_this<NodeType>,
        NodeType
        >::value
      > impl;
    impl(safe_cast<const NodeType>(this), V);
  }
}

#endif
