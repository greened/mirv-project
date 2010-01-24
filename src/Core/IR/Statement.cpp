#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>

namespace mirv{
  void Statement<Base>::accept(StatementVisitor &V) {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<Statement<Base> >::type p = safe_cast<Statement<Base> >(pn);
    V.visit(p);
  }
#if 0
  void Statement<Inner<detail::InnerStatementTraits> >::accept(StatementVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<Statement<Base> >::type p = safe_cast<Statement<Base> >(pn);
    V.visit(p);
  }
#endif
  void InnerStatement::accept(StatementVisitor &V) {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<InnerStatement>::type p = safe_cast<InnerStatement>(pn);
    V.visit(p);
  }

  void LeafStatement::accept(StatementVisitor &V) {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<LeafStatement>::type p = safe_cast<LeafStatement>(pn);
    V.visit(p);
  }
}
