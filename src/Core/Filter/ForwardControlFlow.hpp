#ifndef mirv_Core_Filter_ForwardControlFlow_hpp
#define mirv_Core_Filter_ForwardControlFlow_hpp

#include <mirv/Core/Filter/ControlFlow.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/Memory/Heap.hpp>

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
    typename BetweenExprAction,
    typename ExprAction,
    typename AfterExprAction
    >
  class ForwardFlow
      : public ControlFlow<
    EnterAction,
    LeaveAction,
    BeforeStmtAction,
    AfterStmtAction,
    BetweenStmtAction,
    JoinAction,
    BeforeExprAction,
    BetweenExprAction,
    ExprAction,
    AfterExprAction
    > {
    typedef ControlFlow<
      EnterAction,
      LeaveAction,
      BeforeStmtAction,
      AfterStmtAction,
      BetweenStmtAction,
      JoinAction,
      BeforeExprAction,
      BetweenExprAction,
      ExprAction,
      AfterExprAction
      > BaseType;

  public:
    ForwardFlow(const EnterAction &e,
                const LeaveAction &l,
                const BeforeStmtAction &bs,
                const AfterStmtAction &as,
                const BetweenStmtAction &bts,
                const JoinAction &j,
                const BeforeExprAction &be,
                const BetweenExprAction &bte,
                const ExprAction &ex,
                const AfterExprAction &ae)
      : BaseType(e, l, bs, as, bts, j, be, bte, ex, ae) {}

    // Allow in-place construction of actions.
    template<typename ...Args>
    ForwardFlow(Args &...args) : BaseType(args...) {}

    /// Visit Block statements from the first child to the last.
    void visit(Block & stmt) override {
      this->doEnter(stmt);

      ptr<Control> Prev = nullptr;
      for(auto S : stmt.Statements()) {
        if (Prev) {
          this->doBetweenControl(stmt, Prev, S);
        }

        this->doBeforeControl(stmt, S);
        S->accept(*this);
        this->doAfterControl(stmt, S);

        Prev = S;
      }
      this->doLeave(stmt);
    }

    /// Visit IfThen statements, starting with the controlling
    /// expression, followed by the then body.
    void visit(IfThen & stmt) override {
      this->doEnter(stmt);

      this->doBeforeExpression(stmt, stmt.getCondition());
      this->doExpression(stmt, stmt.getCondition());
      this->doAfterExpression(stmt, stmt.getCondition());

      this->doBeforeControl(stmt, stmt.getThenBlock());
      stmt.getThenBlock()->accept(*this);
      this->doAfterControl(stmt, stmt.getThenBlock());

      this->doJoin(stmt, stmt.getThenBlock());

      this->doLeave(stmt);
    }

    /// Visit IfElse statements, starting with the controllingiled
    /// expression, followed by the then body, then the else body.
    void visit(IfElse & stmt) override {
      this->doEnter(stmt);

      this->doBeforeExpression(stmt, stmt.getCondition());
      this->doExpression(stmt, stmt.getCondition());
      this->doAfterExpression(stmt, stmt.getCondition());

      this->doBeforeControl(stmt, stmt.getThenBlock());
      stmt.getThenBlock()->accept(*this);
      this->doAfterControl(stmt, stmt.getThenBlock());

      this->doBetweenControl(stmt, stmt.getThenBlock(), stmt.getElseBlock());

      this->doBeforeControl(stmt, stmt.getElseBlock());
      stmt.getElseBlock()->accept(*this);
      this->doAfterControl(stmt, stmt.getElseBlock());

      this->doJoin(stmt, stmt.getThenBlock(), stmt.getElseBlock());

      this->doLeave(stmt);
    }

    /// Visit DoWhile statements, starting with the body, followed by
    /// the controlling expression.
    void visit(DoWhile & stmt) override {
      this->doEnter(stmt);
      bool iterate=false;
      do {
        this->doBeforeControl(stmt, stmt.getBody());
        stmt.getBody()->accept(*this);
        this->doAfterControl(stmt, stmt.getBody());

        this->doJoin(stmt,
                     stmt.getCondition(),
                     stmt.getBody());
        this->doBeforeExpression(stmt, stmt.getCondition());
        this->doExpression(stmt, stmt.getCondition());
        this->doAfterExpression(stmt, stmt.getCondition());
      } while (iterate);

      this->doLeave(stmt);
    }

    /// Visit Switch statements, starting with the controlling
    /// expression, followed by the cast statements.
    void visit(Switch & stmt) override {
      this->doEnter(stmt);

      this->doBeforeExpression(stmt, stmt.getSelector());
      this->doExpression(stmt, stmt.getSelector());
      this->doAfterExpression(stmt, stmt.getSelector());

      ptr<Control> Prev = nullptr;
      for (auto Case : stmt.Cases()) {
        if (Prev) {
          this->doBetweenControl(stmt, Prev, Case);
        }

        this->doBeforeControl(stmt, Case);
        Case->accept(*this);
        this->doAfterControl(stmt, Case);

        // Join point: exits of case statements.
        if (Prev) {
          this->doJoin(Case,  Prev);
        }

        Prev = Case;
      }

      this->doLeave(stmt);
    }

    /// Visit Case statements, starting with the controlling
    /// expression, followed by the body.
    void visit(Case & stmt) override {
      this->doEnter(stmt);

      this->doBeforeExpression(stmt, stmt.getCondition());
      this->doExpression(stmt, stmt.getCondition());
      this->doAfterExpression(stmt, stmt.getCondition());

      this->doBeforeControl(stmt, stmt.getBlock());
      stmt.getBlock()->accept(*this);
      this->doAfterControl(stmt, stmt.getBlock());

      this->doLeave(stmt);
    }

    /// Visit Goto statements.
    void visit(Goto & stmt) override {
      this->doEnter(stmt);

      // Join point: Control and jump target.
      // TODO: We need to remember to iterate if state changes.
      //bool iterate = this->doJoin(stmt, stmt.getTarget());
      this->doLeave(stmt);
    }

    /// Visit Sequence statements.
    void visit(Sequence & stmt) override {
      this->doEnter(stmt);

      this->doBeforeExpression(stmt, stmt.getProducer());
      this->doExpression(stmt, stmt.getProducer());
      this->doAfterExpression(stmt, stmt.getProducer());

      this->doLeave(stmt);
    }

    /// Visit SubroutineCall statements.
    void visit(SubroutineCall & stmt) override {
      this->doEnter(stmt);

      ptr<ValueProducer> Prev = nullptr;
      for (auto P : stmt.Operands()) {
        if (Prev) {
          this->doBetweenExpression(stmt, Prev, P);
        }
        this->doBeforeExpression(stmt, P);
        this->doExpression(stmt, P);
        this->doAfterExpression(stmt, P);
        Prev = P;
      }

      this->doLeave(stmt);
    }

    /// Visit Return statements.
    void visit(Return & stmt) override {
      this->doEnter(stmt);
      this->doLeave(stmt);
    }
  };

  /// This is a type generator to create forward flow types.
  struct ForwardFlowGenerator {
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
    /// This is a metafunction to generate a ForwardFlow type with the
    /// given configutation.
    struct apply {
      typedef ForwardFlow<
        EnterAction,
        LeaveAction,
        BeforeStmtAction,
        AfterStmtAction,
        BetweenStmtAction,
        JoinAction,
        BeforeExprAction,
        BetweenExprAction,
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
    typename BetweenExprAction,
    typename ExprAction,
    typename AfterExprAction
    >
  typename ptr<ForwardFlow<
                 EnterAction,
                 LeaveAction,
                 BeforeStmtAction,
                 AfterStmtAction,
                 BetweenStmtAction,
                 JoinAction,
                 BeforeExprAction,
                 BetweenExprAction,
                 ExprAction,
                 AfterExprAction
                 > >::type
  makeForwardFlow(const EnterAction &ent,
                  const LeaveAction &lv,
                  const BeforeStmtAction &bs,
                  const AfterStmtAction &as,
                  const BetweenStmtAction &bts,
                  const JoinAction &j,
                  const BeforeExprAction &be,
                  const BetweenExprAction &bte,
                  const ExprAction &e,
                  const AfterExprAction &ae) {
    return new ForwardFlow<
      EnterAction,
      LeaveAction,
      BeforeStmtAction,
      AfterStmtAction,
      BetweenStmtAction,
      JoinAction,
      BeforeExprAction,
      BetweenExprAction,
      ExprAction,
      AfterExprAction
      >(ent, lv, bs, as, bts, j, be, bte, e, ae);
  }
}

#endif
