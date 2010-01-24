#ifndef mirv_Core_IR_Statement_ipp
#define mirv_Core_IR_Statement_ipp

#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>

#include <mirv/Core/Filter/StatementVisitor.hpp>

namespace mirv {
  template<typename Tag>
  void Statement<Tag>::accept(StatementVisitor &V) {
    ptr<Node<Base> >::type pn = this->shared_from_this();
    typename ptr<Statement<Tag> >::type p = safe_cast<Statement<Tag> >(pn);
    V.visit(p);
  }
}

#endif
