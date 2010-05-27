// -*-C++-*-
#ifndef mirv_Core_IR_Statement_ipp
#define mirv_Core_IR_Statement_ipp

#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>

#include <mirv/Core/Filter/StatementVisitor.hpp>
#include <mirv/Core/Filter/NodeVisitor.hpp>

#include <boost/type_traits.hpp>

namespace mirv {
  template<typename Tag>
  void Statement<Tag>::accept(StatementVisitor &V) {
    detail::AcceptImpl<Statement<Tag>,
      boost::is_base_of<boost::enable_shared_from_this<Statement<Tag> >,
      Statement<Tag> >::value> impl;
    impl(this, V);
  }
}

#endif
