#ifndef mirv_Core_Filter_BackwardControlFlow_hpp
#define mirv_Core_Filter_BackwardControlFlow_hpp

#include <mirv/Core/Filter/ControlFlow.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/Memory/Heap.hpp>

namespace mirv {
  /// This flow walks backward through statements, visiting rightmost
  /// operands first.
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
   class BackwardFlow
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
     typedef Map<ptr<Block>, bool> LabelIterateMapType;
     LabelIterateMapType iterateMap;

   public:
     BackwardFlow(const EnterAction &e,
                  const LeaveAction &l,
                  const BeforeStmtAction &bs,
                  const AfterStmtAction &as,
                  const JoinAction &j,
                  const BetweenStmtAction &bts,
                  const BeforeExprAction &be,
                  const BetweenExprAction &bte,
                  const ExprAction &ex,
                  const AfterExprAction &ae)
       : ControlFlow(e, l, bs, as, bts, j, be, bte, ex, ae) {}

    // Allow in-place construction of actions.
    template<typename ...Args>
    BackwardFlow(Args &...args) : BaseType(args...) {}

     /// Visit block statements from the last statement to the first.
     void visit(Block & stmt) {
       this->doEnter(stmt);
       ptr<Control> Prev = nullptr;
       for(auto S : stmt.RStatements()) {
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

     /// Visit IfThen statements, visiting the then-statement, then
     /// the controlling expression.
     void visit(IfThen & stmt) {
       this->doEnter(stmt);

       this->doBeforeControl(stmt, stmt.getThenBlock());
       stmt.getThenBlock()->accept(*this);
       this->doAfterControl(stmt, stmt.getThenBlock());

       this->doJoin(stmt, stmt.getThenBlock());

       this->doBeforeExpression(stmt, stmt.getCondition());
       this->doExpression(stmt, stmt.getCondition());
       this->doAfterExpression(stmt, stmt.getCondition());

       this->doLeave(stmt);
     }

     /// Visit IfElse statements, visiting the then-statement, then
     /// the else-statement, then the controlling expression.
     void visit(IfElse & stmt) {
       this->doEnter(stmt);

       this->doBeforeControl(stmt, stmt.getThenBlock());
       stmt.getThenBlock()->accept(*this);
       this->doAfterControl(stmt, stmt.getThenBlock());

       this->doBetweenControl(stmt, stmt.getThenBlock(), stmt.getElseBlock());

       this->doBeforeControl(stmt, stmt.getElseBlock());
       stmt.getElseBlock()->accept(*this);
       this->doAfterControl(stmt, stmt.getElseBlock());

       this->doJoin(stmt, stmt.getThenBlock(), stmt.getElseBlock());

       this->doBeforeExpression(stmt, stmt.getCondition());
       this->doExpression(stmt, stmt.getCondition());
       this->doAfterExpression(stmt, stmt.getCondition());

       this->doLeave(stmt);
     }

     /// Visit DoWhile statements, visiting the controlling
     /// expression, then the body.
     void visit(DoWhile & stmt) {
       this->doEnter(stmt);

       do {
         this->doBeforeExpression(stmt, stmt.getCondition());
         this->doExpression(stmt, stmt.getCondition());
         this->doAfterExpression(stmt, stmt.getCondition());

         this->doBeforeControl(stmt, stmt.getBody());
         stmt.getBody()->accept(*this);
         this->doAfterControl(stmt, stmt.begin());
       } while (this->doJoin(stmt,
                             stmt.getCondition(),
                             stmt.getBody()));

       this->doLeave(stmt);
     }

     /// Visit Switch statements, visiting the cases in reverse order,
     /// then the controlling expression.
     void visit(Switch & stmt) {
       this->doEnter(stmt);

       ptr<Control> Prev = nullptr;
       for (auto Case : stmt.Cases()) {
         if (Prev) {
           this->doBetweenControl(stmt, Prev, Case);
         }

         this->doBeforeControl(stmt, Case);
         Case->accept(*this);
         this->doAfterControl(stmt, Case);

         // Join point: entrances of case statements.
         if (Prev) {
           this->doJoin(Case,  Prev);
         }

         Prev = Case;
       }

       this->doBeforeExpression(stmt, stmt.getSelector());
       this->doExpression(stmt, stmt.getCondition());
       this->doAfterExpression(stmt, stmt.getSelector());

       this->doLeave(stmt);
     }

     /// Visit Case statements, visiting the body, then the
     /// controlling expression.
     void visit(Case & stmt) {
       this->doEnter(stmt);

       this->doBeforeControl(stmt, stmt.getBlock());
       stmt.getBlock()->accept(*this);
       this->doAfterControl(stmt, stmt.getBlock());

       this->doBeforeExpression(stmt, stmt.getCondition());
       this->doExpression(stmt, stmt.getCondition());
       this->doAfterExpression(stmt, stmt.getCondition());

       this->doLeave(stmt);
     }

     /// Visit Goto statements.
     void visit(Goto & stmt) {
       this->doEnter(stmt);

       // Join point: Jump and target.
       // TODO: We need to remember to iterate if state changes.
       bool iterate = this->doJoin(stmt, stmt.getTarget());

       this->doLeave(stmt);
     }

     /// Visit Sequence statements.
     void visit(Sequence & stmt) {
       this->doEnter(stmt);

       this->doBeforeExpression(stmt, stmt.getProducer());
       this->doExpression(stmt, stmt.getProducer());
       this->doAfterExpression(stmt, stmt.getProducer());

       this->doLeave(stmt);
     }

     /// Visit SubroutineCall statements.
     void visit(SubroutineCall & stmt) {
       this->doEnter(stmt);

       ptr<ValueProducer> Prev = nullptr;
       for (auto &D : stmt.Operands()) {
         auto P = D.getProducer();
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
     void visit(Return & stmt) {
       this->doEnter(stmt);
       this->doLeave(stmt);
     }
   };

  /// This is a type generator to create forward flow types.
  struct BackwardFlowGenerator {
    template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeStmtAction,
      typename AfterStmtAction,
      typename BetweenStmtAction,
      typename BeforeExprAction,
      typename ExprAction,
      typename AfterExprAction
      >
    /// This is the metafunction to generate a BackwardFlow type with
    /// the given configuration.
    struct apply {
      typedef BackwardFlow<
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
}

#endif
