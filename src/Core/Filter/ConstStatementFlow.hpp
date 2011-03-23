#ifndef mirv_Core_Filter_ConstStatementFlow_hpp
#define mirv_Core_Filter_ConstStatementFlow_hpp

#include <mirv/Core/Filter/ConstStatementVisitor.hpp>
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
    typename ExprAction,
    typename AfterExprAction
    >
  class ConstStatementFlow : public ConstStatementVisitor {
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
    /// The action to apply when visiting a child expression.
    ExprAction expr;
    /// The action to apply after visiting a child expression.
    AfterExprAction aftexpr;

  protected:
    EnterAction &enterStatement(void) {
      return ent;
    }

    EnterAction &leaveStatement(void) {
      return lve;
    }

    BeforeStmtAction &beforeStatement(void) {
      return bfrstmt;
    }

    AfterStmtAction &afterStatement(void) {
      return aftstmt;
    }

    BetweenStmtAction &betweenStatement(void) {
      return betstmt;
    }

    JoinAction &join(void) {
      return jn;
    }

    BeforeExprAction &beforeExpression(void) {
      return bfrexpr;
    }

    ExprAction &expression(void) {
      return expr;
    }

    AfterExprAction &afterExpression(void) {
      return aftexpr;
    }

    /// Apply the enter action.
    template<typename Stmt>
    typename EnterAction::result_type doEnter(boost::shared_ptr<Stmt> stmt) {
      return ent(stmt);
    }

    /// Apply the leave action.
    template<typename Stmt>
    typename LeaveAction::result_type doLeave(boost::shared_ptr<Stmt> stmt) {
      return lve(stmt);
    }

    /// Apply the before action.
    template<typename Stmt, typename InputIterator>
    typename BeforeStmtAction::result_type doBeforeStatement(boost::shared_ptr<Stmt> stmt,
                                                             InputIterator child) {
      return bfrstmt(stmt, child);
    }

    /// Apply the after action.
    template<typename Stmt, typename InputIterator>
    typename AfterStmtAction::result_type doAfterStatement(boost::shared_ptr<Stmt> stmt,
                                                           InputIterator child) {
      return aftstmt(stmt, child);
    }

    /// Apply the between action.
    template<typename Stmt, typename InputIterator>
    typename BetweenStmtAction::result_type doBetweenStatement(boost::shared_ptr<Stmt> stmt,
                                                               InputIterator child1,
                                                               InputIterator child2) {
      return betstmt(stmt, child1, child2);
    }

    /// Apply the join action.
    template<typename Stmt, typename InputIterator>
    typename JoinAction::result_type doJoin(boost::shared_ptr<Stmt> stmt,
                                            InputIterator child) {
      return jn(stmt, child);
    }

    /// Apply the join action.
    template<typename Stmt, typename InputIterator1, typename InputIterator2>
    typename JoinAction::result_type doJoin(boost::shared_ptr<Stmt> stmt,
                                            InputIterator1 child1,
                                            InputIterator2 child2) {
      return jn(stmt, child1, child2);
    }

    /// Apply the before expression action.
    template<typename Stmt, typename InputIterator>
    typename BeforeExprAction::result_type doBeforeExpression(boost::shared_ptr<Stmt> stmt,
                                                              InputIterator expr) {
      return bfrexpr(stmt, expr);
    }

    /// Apply the expression action.
    template<typename Stmt, typename InputIterator>
    typename ExprAction::result_type doExpression(boost::shared_ptr<Stmt> stmt,
                                                  InputIterator exp) {
      return expr(stmt, exp);
    }

    /// Apply the after expression action.
    template<typename Stmt, typename InputIterator>
    typename AfterExprAction::result_type doAfterExpression(boost::shared_ptr<Stmt> stmt,
                                                            InputIterator expr) {
      return aftexpr(stmt, expr);
    }

  public:
    ConstStatementFlow(const EnterAction &e,
                       const LeaveAction &l,
                       const BeforeStmtAction &bs,
                       const AfterStmtAction &as, 
                       const BetweenStmtAction &bt,
                       const JoinAction &j,
                       const BeforeExprAction &be,
                       const ExprAction &ex,
                       const AfterExprAction &ae)
        : ent(e), lve(l), bfrstmt(bs), aftstmt(as), betstmt(bt), jn(j),
            bfrexpr(be), expr(ex), aftexpr(ae) {}

    // Allow in-place construction of actions.
    template <typename ...Args>
    ConstStatementFlow(Args &...args)
        : ent(args...), lve(args...), bfrstmt(args...), aftstmt(args...),
            betstmt(args...), jn(args...), bfrexpr(args...), expr(args...),
            aftexpr(args...) {}

    template<typename Flow>
    void transfer(Flow &other) {}
  };

  typedef ConstStatementFlow<
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction
    > NullConstStatementFlow;
}

#endif
