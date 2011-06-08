#ifndef mirv_Core_Filter_ConstForwardStatementFlow_hpp
#define mirv_Core_Filter_ConstForwardStatementFlow_hpp

#include <mirv/Core/Filter/ConstStatementFlow.hpp>
#include <mirv/Core/IR/Attribute.hpp>
#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Containers/Map.hpp>

namespace mirv {
  /// This is a statement flow that walks the IR in execution order.
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
  class ConstForwardStatementFlow
      : public ConstStatementFlow<
    EnterAction,
    LeaveAction,
    BeforeStmtAction,
    AfterStmtAction,
    BetweenStmtAction,
    JoinAction,
    BeforeExprAction,
    ExprAction,
    AfterExprAction
    > {
    typedef ConstStatementFlow<
      EnterAction,
      LeaveAction,
      BeforeStmtAction,
      AfterStmtAction, 
      BetweenStmtAction,
      JoinAction,
      BeforeExprAction,
      ExprAction,
      AfterExprAction
      > BaseType;

    typedef Map<ptr<Expression<Base>>::const_type, bool>::type LabelIterateMapType;
    LabelIterateMapType iterateMap;

  public:
    ConstForwardStatementFlow(const EnterAction &e,
                              const LeaveAction &l,
                              const BeforeStmtAction &bs,
                              const AfterStmtAction &as,
                              const BetweenStmtAction &bts,
                              const JoinAction &j,
                              const BeforeExprAction &be,
                              const ExprAction &ex,
                              const AfterExprAction &ae)
        : BaseType(e, l, bs, as, bts, j, be, ex, ae) {}

    // Allow in-place construction of actions.
    template<typename ...Args>
    ConstForwardStatementFlow(Args &...args) : BaseType(args...) {}

    /// Visit Block statements from the first child to the last.
    void visit(ptr<Statement<Block> >::const_type stmt) {
      this->doEnter(stmt);
      for(Statement<Block>::const_iterator s = stmt->begin(),
            send = stmt->end();
          s != send;
          /* NULL */) {
        this->doBeforeStatement(stmt, s);
        (*s)->accept(*this);
        this->doAfterStatement(stmt, s);
        Statement<Block>::const_iterator prev = s;
        if (++s != send) {
          this->doBetweenStatement(stmt, prev, s);
        }
      }
      this->doLeave(stmt);
    }

    /// Visit IfThen statements, starting with the controlling
    /// expression, followed by the then body.
    void visit(ptr<Statement<IfThen> >::const_type stmt) {
      this->doEnter(stmt);

      this->doBeforeExpression(stmt, stmt->expressionBegin());
      this->doExpression(stmt, stmt->expressionBegin());
      this->doAfterExpression(stmt, stmt->expressionBegin());

      this->doBeforeStatement(stmt, stmt->begin());
      stmt->getChildStatement()->accept(*this);
      this->doAfterStatement(stmt, stmt->begin());

      this->doJoin(stmt, stmt->begin());

      this->doLeave(stmt);
    }

    /// Visit IfElse statements, starting with the controllingiled
    /// expression, followed by the then body, then the else body.
    void visit(ptr<Statement<IfElse> >::const_type stmt) {
      this->doEnter(stmt);

      this->doBeforeExpression(stmt, stmt->expressionBegin());
      this->doExpression(stmt, stmt->expressionBegin());
      this->doAfterExpression(stmt, stmt->expressionBegin());

      Statement<IfElse>::const_iterator s = stmt->begin();

      this->doBeforeStatement(stmt, *s);
      (*s)->accept(*this);
      this->doAfterStatement(stmt, *s);

      Statement<IfElse>::const_iterator prev = s++;

      this->doBetweenStatement(stmt, *prev, *s);

      this->doBeforeStatement(stmt, *s);
      (*s)->accept(*this);
      this->doAfterStatement(stmt, *s);

      this->doJoin(stmt, *prev, *s);

      this->doLeave(stmt);
    }

    /// Visit While statements, starting with the controlling
    /// expression, followed by the body.
    void visit(ptr<Statement<While> >::const_type stmt) {
      this->doEnter(stmt);

      this->doBeforeExpression(stmt, stmt->expressionBegin());
      this->doExpression(stmt, stmt->expressionBegin());
      this->doAfterExpression(stmt, stmt->expressionBegin());

      bool iterate = false;

      do {
        this->doBeforeStatement(stmt, stmt->begin());
        stmt->getChildStatement()->accept(*this);
        this->doAfterStatement(stmt, stmt->begin());

        // Join point: Loop body and expression.
        iterate = this->doJoin(stmt,
                               stmt->expressionBegin(),
                               stmt->begin());

        this->doBeforeExpression(stmt, stmt->expressionBegin());
        this->doExpression(stmt, stmt->expressionBegin());
        this->doAfterExpression(stmt, stmt->expressionBegin());
      } while (iterate);

      this->doLeave(stmt);
    }

    /// Visit DoWhile statements, starting with the body, followed by
    /// the controlling expression.
    void visit(ptr<Statement<DoWhile> >::const_type stmt) {
      this->doEnter(stmt);

      do {
        this->doBeforeStatement(stmt, stmt->begin());
        stmt->getChildStatement()->accept(*this);
        this->doAfterStatement(stmt, stmt->begin());

        this->doBeforeExpression(stmt, stmt->expressionBegin());
        this->doExpression(stmt, stmt->expressionBegin());
        this->doAfterExpression(stmt, stmt->expressionBegin());
      } while (this->doJoin(stmt->getExpression(),
                            stmt->begin()));

      this->doLeave(stmt);
    }

    /// Visit Switch statements, starting with the controlling
    /// expression, followed by the cast statements.
    void visit(ptr<Statement<Switch> >::const_type stmt) {
      this->doEnter(stmt);

      this->doBeforeExpression(stmt, stmt->expressionBegin());
      this->doExpression(stmt, stmt->expressionBegin());
      this->doAfterExpression(stmt, stmt->expressionBegin());

      for(Statement<Switch>::const_iterator s = stmt->begin(),
            send = stmt->end();
          s != send;
          /* NULL */) {
        this->doBeforeStatement(stmt, s);
        (*s)->accept(*this);
        this->doAfterStatement(stmt, s);
        Statement<Block>::const_iterator prev = s;
        if (++s != send) {
          this->doBetweenStatement(stmt, prev, s);
          // Joint point: jump to label or fall-through.
          this->doJoin(stmt->getExpression(), s);
        }
      }
      // Join point: exits of case statements.  Handled at jumps.         
      this->doLeave(stmt);
    }

    /// Visit Case statements, starting with the controlling
    /// expression, followed by the body.
    void visit(ptr<Statement<Case> >::const_type stmt) {
      this->doEnter(stmt);

      this->doBeforeExpression(stmt, stmt->expressionBegin());
      this->doExpression(stmt, stmt->expressionBegin());
      this->doAfterExpression(stmt, stmt->expressionBegin());

      this->doBeforeStatement(stmt, stmt->begin());
      stmt->getChildStatement()->accept(*this);
      this->doAfterStatement(stmt, stmt->begin());

      this->doLeave(stmt);
    }

    void visit(ptr<Statement<CaseBlock> >::const_type stmt) {
    }

    /// Visit Before statements.
    void visit(ptr<Statement<Before> >::const_type stmt) {
      this->doEnter(stmt);

      LabelIterateMapType::iterator i;
      bool inserted = false;

      tie(i, inserted) =
        iterateMap.insert(std::make_pair(stmt->getExpression(), false));
      checkInvariant(inserted, "Duplicate label in iterate map");

      do {
        this->doBeforeStatement(stmt, stmt->begin());
        stmt->getChildStatement()->accept(*this);
        this->doAfterStatement(stmt, stmt->begin());
        // Joint point: Begin of before statement and jump to
        // label.  Handled at jump.
      } while(i->second);

      iterateMap.erase(i);

      this->doLeave(stmt);
    }

    /// Visit After statements.
    void visit(ptr<Statement<After> >::const_type stmt) {
      this->doEnter(stmt);

      this->doBeforeStatement(stmt, stmt->begin());
      stmt->getChildStatement()->accept(*this);
      this->doAfterStatement(stmt, stmt->begin());
      // Join point: Body and jump.  Handled at jump.

      this->doLeave(stmt);
    }

    /// Visit Goto statements.
    void visit(ptr<Statement<Goto> >::const_type stmt) {
      this->doEnter(stmt);

      LabelIterateMapType::iterator i =
        iterateMap.find(stmt->getExpression());
         
      // Join point: Statement and jump target.
      bool iterate = this->doJoin(stmt, stmt->expressionBegin());
      if (i != iterateMap.end()) {
        i->second = iterate;
      }

      this->doLeave(stmt);
    }

    /// Visit Return statements.
    void visit(ptr<Statement<Return> >::const_type stmt) {
      this->doEnter(stmt);
      this->doLeave(stmt);
    }

    /// Visit Assignment statements, starting with the right-hand
    /// side, followed by the left-hand side.
    void visit(ptr<Statement<Assignment> >::const_type stmt) {
      this->doEnter(stmt);

      for (auto i = stmt->expressionRBegin();
           i != stmt->expressionREnd();
           ++i) {
        this->doBeforeExpression(stmt, i);
        this->doExpression(stmt, i);
        this->doAfterExpression(stmt, i);
      }

      this->doLeave(stmt);
    }

    /// Visit Call statements.
    void visit(ptr<Statement<Call> >::const_type stmt) {
      this->doEnter(stmt);

      for (auto e = stmt->expressionBegin(); e != stmt->expressionEnd(); ++e) {
        this->doBeforeExpression(stmt, e);
        this->doExpression(stmt, e);
        this->doAfterExpression(stmt, e);
      }

      this->doLeave(stmt);
    }

    /// Visit Allocate statements, starting with the right-hand
    /// side, followed by the left-hand side.
    void visit(ptr<Statement<Allocate> >::const_type stmt) {
      this->doEnter(stmt);

      for (auto e = stmt->expressionRBegin(); e != stmt->expressionREnd(); ++e) {
        this->doBeforeExpression(stmt, e);
        this->doExpression(stmt, e);
        this->doAfterExpression(stmt, e);
      }

      this->doLeave(stmt);
    }
  };

  /// This is a type generator to create forward flow types.
  struct ConstForwardStatementFlowGenerator {
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
    /// This is a metafunction to create a ConstForwardStatementFlow
    /// with the given configuration.
    struct apply {
      typedef ConstForwardStatementFlow<
        EnterAction,
        LeaveAction,
        BeforeStmtAction, 
        AfterStmtAction,
        BetweenStmtAction,
        JoinAction,
        BeforeExprAction,
        ExprAction,
        AfterExprAction
        > type;
    };
  };

  /// This is a generator function to create forward flows.  It uses
  /// argument type deduction to relieve the user of the need to
  /// specify all of the template arguments.
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
  typename ptr<ConstForwardStatementFlow<
                 EnterAction,
                 LeaveAction,
                 BeforeStmtAction,
                 AfterStmtAction,
                 BetweenStmtAction,
                 JoinAction,
                 BeforeExprAction,
                 ExprAction,
                 AfterExprAction
                 > >::const_type
  makeConstForwardStatementFlow(const EnterAction &ent,
                                const LeaveAction &lv,
                                const BeforeStmtAction &bs,
                                const AfterStmtAction &as,
                                const BetweenStmtAction &bts,
                                const JoinAction &j,
                                const BeforeExprAction &be,
                                const ExprAction &e,
                                const AfterExprAction &ae) {
    return new ConstForwardStatementFlow<
      EnterAction, 
      LeaveAction,
      BeforeStmtAction,
      AfterStmtAction,
      BetweenStmtAction,
      JoinAction,
      BeforeExprAction,
      ExprAction,
      AfterExprAction
      >(ent, lv, bs, as, bts, j, be, e, ae);
  }
}

#endif
