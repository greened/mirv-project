#ifndef mirv_Core_Filter_ControlFlow_hpp
#define mirv_Core_Filter_ControlFlow_hpp

#include <mirv/Core/Filter/ControlVisitor.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/Filter/Dataflow.hpp>
#include <mirv/Core/Memory/Heap.hpp>

namespace mirv {
  /// This is the base class of all flows that walk statement trees.
  template<
    typename EnterAction,
    typename LeaveAction,
    typename BeforeStmtAction,
    typename AfterStmtAction,
    typename BetweenStmtAction,
    typename JoinAction,
    typename BeforeExprAction,
    typename BetweenExprAction,
    typename ExprAction,
    typename AfterExprAction
    >
  class ControlFlow : public ControlVisitor {
  private:
    /// The action to apply upon entering a statement.
    EnterAction ent;
    /// The action to apply upon leaving a statement.
    LeaveAction lve;
    /// The action to apply before visiting a child statement.
    BeforeStmtAction bfrstmt;
    /// The action to apply after visiting a child statement.
    AfterStmtAction aftstmt;
    /// The action to apply between a pair of child statements.
    BetweenStmtAction betstmt;
    /// The action to apply at the join point of control flow.
    JoinAction jn;
    /// The action to apply before visiting a child expression.
    BeforeExprAction bfrexpr;
    /// The action to apply between a pair of child expressions.
    BetweenExprAction betexpr;
    /// The action to apply when visiting a child expression.
    ExprAction expr;
    /// The action to apply after visiting a child expression.
    AfterExprAction aftexpr;

  protected:
    EnterAction &enterControl(void) {
      return ent;
    }

    EnterAction &leaveControl(void) {
      return lve;
    }

    BeforeStmtAction &beforeControl(void) {
      return bfrstmt;
    }

    AfterStmtAction &afterControl(void) {
      return aftstmt;
    }

    BetweenStmtAction &betweenControl(void) {
      return betstmt;
    }

    JoinAction &join(void) {
      return jn;
    }

    BeforeExprAction &beforeExpression(void) {
      return bfrexpr;
    }

    BetweenExprAction &betweenExpression(void) {
      return betexpr;
    }

    ExprAction &expression(void) {
      return expr;
    }

    AfterExprAction &afterExpression(void) {
      return aftexpr;
    }

    /// Apply the enter action.
    template<typename Stmt>
    typename EnterAction::result_type doEnter(Stmt & stmt) {
      return ent(stmt);
    }

    /// Apply the leave action.
    template<typename Stmt>
    typename LeaveAction::result_type doLeave(Stmt & stmt) {
      return lve(stmt);
    }

    /// Apply the before action.
    template<typename Stmt, typename Child>
    typename BeforeStmtAction::result_type doBeforeControl(Stmt & stmt,
                                                           ptr<Child> child) {
      return bfrstmt(stmt, child);
    }

    /// Apply the after action.
    template<typename Stmt, typename Child>
    typename AfterStmtAction::result_type doAfterControl(Stmt & stmt,
                                                         ptr<Child> child) {
      return aftstmt(stmt, child);
    }

    /// Apply the between action.
    template<typename Stmt, typename Child1, typename Child2>
    typename BetweenStmtAction::result_type doBetweenControl(Stmt & stmt,
                                                             ptr<Child1> child1,
                                                             ptr<Child2> child2) {
      return betstmt(stmt, child1, child2);
    }

    /// Apply the join action.
    template<typename Stmt, typename Child>
    typename JoinAction::result_type doJoin(Stmt & stmt,
                                            ptr<Child> child) {
      return jn(stmt, child);
    }

    /// Apply the join action.
    template<typename Stmt, typename Child1, typename Child2>
    typename JoinAction::result_type doJoin(Stmt & stmt,
                                            ptr<Child1> child1,
                                            ptr<Child2> child2) {
      return jn(stmt, child1, child2);
    }

    /// Apply the before expression action.
    template<typename Stmt, typename Child>
    typename BeforeExprAction::result_type doBeforeExpression(Stmt & stmt,
                                                              ptr<Child> expr) {
      return bfrexpr(stmt, expr);
    }

    /// Apply the between expression action.
    template<typename Stmt, typename Child1, typename Child2>
    typename BetweenExprAction::result_type
    doBetweenExpression(Stmt & stmt, ptr<Child1> prev, ptr<Child2> next) {
      return betexpr(stmt, prev, next);
    }

    /// Apply the expression action.
    template<typename Stmt, typename Child>
    typename ExprAction::result_type doExpression(Stmt & stmt,
                                                  ptr<Child> exp) {
      return expr(stmt, exp);
    }

    /// Apply the after expression action.
    template<typename Stmt, typename Child>
    typename AfterExprAction::result_type doAfterExpression(Stmt & stmt,
                                                            ptr<Child> expr) {
      return aftexpr(stmt, expr);
    }

  public:
    ControlFlow(const EnterAction &e,
                  const LeaveAction &l,
                  const BeforeStmtAction &bs,
                  const AfterStmtAction &as,
                  const BetweenStmtAction &bt,
                  const JoinAction &j,
                  const BeforeExprAction &be,
                  const BetweenExprAction &bte,
                  const ExprAction &ex,
                  const AfterExprAction &ae)
      : ent(e), lve(l), bfrstmt(bs), aftstmt(as), betstmt(bt), jn(j),
        bfrexpr(be), betexpr(bte), expr(ex), aftexpr(ae) {}

    // Allow in-place construction of actions.
    template <typename ...Args>
    ControlFlow(Args &...args)
      : ent(args...), lve(args...), bfrstmt(args...), aftstmt(args...),
        betstmt(args...), jn(args...), bfrexpr(args...), betexpr(args...),
        expr(args...), aftexpr(args...) {}

    template<typename Flow>
    void transfer(Flow &other) {}
  };

  typedef ControlFlow<
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction
    > NullControlFlow;
}

#endif
