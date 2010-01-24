#ifndef mirv_core_ir_Statement_ipp
#define mirv_core_ir_Statement_ipp

#include <mirv/core/ir/Statement.hpp>
#include <mirv/core/ir/Control.hpp>
#include <mirv/core/ir/Mutating.hpp>

#include <mirv/core/filter/StatementVisitor.hpp>

namespace mirv {
  template<typename Tag>
  void Statement<Tag>::accept(StatementVisitor &V) {
    ptr<Node<Base> >::type pn = this->shared_from_this();
    typename ptr<Statement<Tag> >::type p = safe_cast<Statement<Tag> >(pn);
    V.visit(p);
  }
}

#endif
