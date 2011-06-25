#include <mirv/Core/Filter/ConstStatementVisitor.hpp>
#include <mirv/Core/Filter/StatementVisitor.hpp>
#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/IR/Visitable.ipp>

namespace mirv{
  void
  Visitable<
    Statement<Base>,
    StatementVisitor
    >::accept(StatementVisitor &V) {
    error("Statement<Base>::accept called");
  }

  void
  ConstVisitable<
    Statement<Base>,
    ConstStatementVisitor,
    StatementVisitor
    >::accept(ConstStatementVisitor &V) const {
    error("Statement<Base>::accept called");
  }

  template
  void
  Visitable<LeafStatement, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<LeafStatement, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Inner<detail::InnerStatementTraits> >, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Inner<detail::InnerStatementTraits> >, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<InnerStatement, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<InnerStatement, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Conditional>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Conditional>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Controlled>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Controlled>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<SingleExpression>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<SingleExpression>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<DualExpression>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<DualExpression>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Iterative>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Iterative>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Mutating>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Mutating>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Block>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Block>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<SingleBlock>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<SingleBlock>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<DualBlock>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<DualBlock>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<SingleCondition>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<SingleCondition>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<IfThen>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<IfThen>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<IfElse>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<IfElse>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<While>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<While>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<DoWhile>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<DoWhile>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<SingleLabel>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<SingleLabel>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Case>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Case>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<CaseBlock>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<CaseBlock>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Switch>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Switch>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Before>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Before>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<After>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<After>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Goto>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Goto>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Return>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Return>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Phi>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Phi>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Assignment>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Assignment>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Call>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Call>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;

  template
  void
  Visitable<Statement<Allocate>, StatementVisitor>::accept(StatementVisitor &);
  template
  void
  ConstVisitable<Statement<Allocate>, ConstStatementVisitor, StatementVisitor>::accept(ConstStatementVisitor &) const;
}
