#ifndef mirv_core_filter_forward_flow_hh
#define mirv_core_filter_forward_flow_hh

#include <mirv/core/filter/statement_flow.hh>
#include <mirv/core/filter/dataflow.hh>
#include <mirv/core/ir/attribute.hh>
#include <mirv/core/ir/statement.hh>
#include <mirv/core/ir/control.hh>
#include <mirv/core/ir/mutating.hh>
#include <mirv/core/mem/heap.hh>

namespace mirv {
  /// This is a statement flow that walks the IR in execution order.
   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeStmtAction = NullAction,
      typename AfterStmtAction = NullAction,
      typename BetweenStmtAction = NullAction,
      typename BeforeExprAction = NullAction,
      typename AfterExprAction = NullAction,
      typename ExprFlow = NullExpressionFlow,
      typename Dataflow = NullDataflow,
      typename Confluence = typename Dataflow::Confluence>
   class ForwardFlow
         : public StatementFlow<
      EnterAction,
      LeaveAction,
      BeforeStmtAction,
      AfterStmtAction,
      BetweenStmtAction,
      BeforeExprAction,
      AfterExprAction,
      ExprFlow,
      Dataflow,
      Confluence> {
     typedef StatementFlow<
      EnterAction,
      LeaveAction,
      BeforeStmtAction,
      AfterStmtAction, 
      BetweenStmtAction,
       BeforeExprAction,
      AfterExprAction,
      ExprFlow,
      Dataflow,
       Confluence> BaseType;

   public:
      ForwardFlow(const EnterAction &e,
                  const LeaveAction &l,
                  const BeforeStmtAction &bs,
                  const AfterStmtAction &as,
                  const BetweenStmtAction &bts,
                  const BeforeExprAction &be,
                  const AfterExprAction &ae,
                  const ExprFlow &expr,
                  const Dataflow &d,
                  const Confluence &c)
	: BaseType(e, l, bs, as, bts, be, ae, expr, d, c) {}

     /// Visit Block statements from the first child to the last.
     void visit(ptr<Statement<Block> >::type stmt) {
         this->enter(stmt);
         for(Statement<Block>::iterator s = stmt->begin(),
                send = stmt->end();
             s != send;
             /* NULL */) {
            this->beforeStatement(stmt, *s);
            (*s)->accept(*this);
            this->afterStatement(stmt, *s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               this->betweenStatement(stmt, *prev, *s);
            }
         }
         this->leave(stmt);
      }

     /// Visit IfThen statements, starting with the controlling
     /// expression, followed by the then body.
      void visit(ptr<Statement<IfThen> >::type stmt) {
         bool hitBreak = this->hasBreak();
         this->enter(stmt);

         this->beforeExpression(stmt, stmt->getExpression());
         stmt->getExpression()->accept(this->expressionFlow());
         this->afterExpression(stmt, stmt->getExpression());

         Dataflow denter(this->dataflow());

         this->setNoBreak();

         this->beforeStatement(stmt, stmt->getChildStatement());
         stmt->getChildStatement()->accept(*this);
         this->afterStatement(stmt, stmt->getChildStatement());

         if (!this->hasBreak()) {
            confluence(this->dataflow(), this->dataflow(), denter);
         }

         this->leave(stmt);
         this->setBreak(hitBreak);
      }

     /// Visit IfElse statements, starting with the controlling
     /// expression, followed by the then body, then the else body.
      void visit(ptr<Statement<IfElse> >::type stmt) {
         bool hitBreak = this->hasBreak();
         this->enter(stmt);

         this->beforeExpression(stmt, stmt->getExpression());
         stmt->getExpression()->accept(this->expressionFlow());
         this->afterExpression(stmt, stmt->getExpression());

         Dataflow denter(this->dataflow());

         this->setNoBreak();
         Statement<IfElse>::iterator s = stmt->begin();

         this->beforeStatement(stmt, *s);
         (*s)->accept(*this);
         this->afterStatement(stmt, *s);

         Dataflow then(this->dataflow());

         bool thenBreak = this->hasBreak();
         this->setNoBreak();

         Statement<IfElse>::iterator prev = s++;

         this->betweenStatement(stmt, *prev, *s);

         this->dataflow() = denter;

         this->beforeStatement(stmt, *s);
         (*s)->accept(*this);
         this->afterStatement(stmt, *s);

         if (this->hasBreak()) {
            if (!thenBreak) {
               this->dataflow() = then;
            }
            else {
               this->dataflow() = denter;
            }
         }
         else if (!thenBreak) {
            confluence(this->dataflow(), this->dataflow(), then);
         }
         // Otherwise we pass through the else clause and don't need
         // to confluence.
         this->leave(stmt);
         this->setBreak(hitBreak);
      }

     /// Visit While statements, starting with the controlling
     /// expression, followed by the body.
      void visit(ptr<Statement<While> >::type stmt) {
         bool hitBreak = this->hasBreak();
         this->enter(stmt);

         Dataflow denter(this->dataflow());

         this->beforeExpression(stmt, stmt->getExpression());
         stmt->getExpression()->accept(this->expressionFlow());
         this->afterExpression(stmt, stmt->getExpression());

         Dataflow firstExpr(this->dataflow());

         do {
            this->beforeStatement(stmt, stmt->getChildStatement());
            stmt->getChildStatement()->accept(*this);
            this->afterStatement(stmt, stmt->getChildStatement());

            if (!this->hasBreak()) {
               confluence(this->dataflow(), this->dataflow(), denter);
            }
            else {
               this->dataflow() = denter;
               break;
            }

            this->beforeExpression(stmt, stmt->getExpression());
            stmt->getExpression()->accept(this->expressionFlow());
            this->afterExpression(stmt, stmt->getExpression());
         } while (this->dataflow().change() && !this->hasBreak());

         if (!this->hasBreak()) {
            // Iterating vs. never enter
            confluence(this->dataflow(), this->dataflow(), firstExpr);
         }

         this->leave(stmt);
         this->setBreak(hitBreak);
      }

     /// Visit DoWhile statements, starting with the body, followed by
     /// the controlling expression.
      void visit(ptr<Statement<DoWhile> >::type stmt) {
         bool hitBreak = this->hasBreak();
         this->enter(stmt);

         Dataflow denter(this->dataflow());

         do {
            this->beforeStatement(stmt, stmt->getChildStatement());
            stmt->getChildStatement()->accept(*this);
            this->afterStatement(stmt, stmt->getChildStatement());

            this->beforeExpression(stmt, stmt->getExpression());
            stmt->getExpression()->accept(this->expressionFlow());
            this->afterExpression(stmt, stmt->getExpression());

            if (!this->hasBreak()) {
               confluence(this->dataflow(), this->dataflow(), denter);
            }
            else {
               this->dataflow() = denter;
               break;
            }
         } while (this->dataflow().change() && !this->hasBreak());

         // Always at least one iteration so no need for confluence
         // here

         this->leave(stmt);
         this->setBreak(hitBreak);
      }

     /// Visit Switch statements, starting with the controlling
     /// expression, followed by the cast statements.
      void visit(ptr<Statement<Switch> >::type stmt) {
         bool hitBreak = this->hasBreak();
         this->setNoBreak();

         this->enter(stmt);

         this->beforeExpression(stmt, stmt->getExpression());
         stmt->getExpression()->accept(this->expressionFlow());
         this->afterExpression(stmt, stmt->getExpression());

         Dataflow denter(this->dataflow());

         for(Statement<Switch>::iterator s = stmt->begin(),
                send = stmt->end();
             s != send;
             /* NULL */) {
            this->beforeStatement(stmt, *s);
            (*s)->accept(*this);
            this->afterStatement(stmt, *s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               this->betweenStatement(stmt, *prev, *s);

               if (this->hasBreak()) {
                  // Don't propagate dataflow from this block to the next
                  // one
                  this->dataflow() = denter;
                  this->setNoBreak();
                  // Exit confluence handled at after statement
               }
               else {
                  // Fall-through vs. direct jump
                  confluence(this->dataflow(), this->dataflow(), denter);
               }
            }
         }

         this->leave(stmt);
         this->setBreak(hitBreak);
      }

     /// Visit Case statements, starting with the controlling
     /// expression, followed by the body.
      void visit(ptr<Statement<Case> >::type stmt) {
         this->enter(stmt);

         this->beforeExpression(stmt, stmt->getExpression());
         stmt->getExpression()->accept(this->expressionFlow());
         this->afterExpression(stmt, stmt->getExpression());

         this->beforeStatement(stmt, stmt->getChildStatement());
         stmt->getChildStatement()->accept(*this);
         this->afterStatement(stmt, stmt->getChildStatement());

         this->leave(stmt);
      }

     void visit(ptr<Statement<CaseBlock> >::type stmt) {
     }

     /// Visit Before statements.
      void visit(ptr<Statement<Before> >::type stmt) {
         this->enter(stmt);

         typename ptr<Dataflow>::type label = getAttribute<Dataflow>(stmt);

         do {
            confluence(this->dataflow(), this->dataflow(), *label);

            this->beforeStatement(stmt, stmt->getChildStatement());
            stmt->getChildStatement()->accept(*this);
            this->afterStatement(stmt, stmt->getChildStatement());
         } while(label->change() || this->dataflow().change());

         this->leave(stmt);
      }

     /// Visit After statements.
      void visit(ptr<Statement<After> >::type stmt) {
         this->enter(stmt);

         this->beforeStatement(stmt, stmt->getChildStatement());
         stmt->getChildStatement()->accept(*this);
         this->afterStatement(stmt, stmt->getChildStatement());

         typename ptr<Dataflow>::type label = getAttribute<Dataflow>(stmt);

         confluence(this->dataflow(), this->dataflow(), *label);

         this->leave(stmt);
      }

     /// Visit Goto statements.
      void visit(ptr<Statement<Goto> >::type stmt) {
         this->enter(stmt);

         typename ptr<Dataflow>::type label = getAttribute<Dataflow>(stmt->getLabel());
         confluence(*label, *label, this->dataflow());

         this->leave(stmt);
      }

     /// Visit Return statements.
      void visit(ptr<Statement<Return> >::type stmt) {
         this->enter(stmt);
         this->leave(stmt);
      }

     /// Visit Assignment statements, starting with the right-hand
     /// side, followed by the left-hand side.
      void visit(ptr<Statement<Assignment> >::type stmt) {
         this->enter(stmt);

         this->beforeExpression(stmt, stmt->getRightExpression());
         stmt->getRightExpression()->accept(this->expressionFlow());
         this->afterExpression(stmt, stmt->getRightExpression());

         this->beforeExpression(stmt, stmt->getLeftExpression());
         stmt->getLeftExpression()->accept(this->expressionFlow());
         this->afterExpression(stmt, stmt->getLeftExpression());

         this->leave(stmt);
      }
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
      typename BeforeExprAction,
      typename AfterExprAction,
      typename ExprFlow,
      typename Dataflow,
      typename Confluence>
   typename ptr<ForwardFlow<EnterAction, LeaveAction, BeforeStmtAction, 
			    AfterStmtAction, BetweenStmtAction,
			    BeforeExprAction, AfterExprAction, ExprFlow,
			    Dataflow, Confluence> >::type
   makeForwardFlow(const EnterAction &ent, const LeaveAction &lv,
		   const BeforeStmtAction &bs,
		   const AfterStmtAction &as, const BetweenStmtAction &bts,
		   const BeforeExprAction &be, const AfterExprAction &ae,
		   const ExprFlow &ef, const Dataflow &df = NullDataflow(),
		   const Confluence &cf = Dataflow::Confluence()) {
     return new ForwardFlow<
     EnterAction, 
       LeaveAction,
       BeforeStmtAction,
       AfterStmtAction,
       BetweenStmtAction,
       BeforeExprAction,
       AfterExprAction,
       ExprFlow,
       Dataflow,
       Confluence>(ent, lv, bs, as, bts, be, ae, ef, df, cf);
   }

  /// This is a generator function to create forward flows.  It uses
  /// argument type deduction to relieve the user of the need to
  /// specify all of the template arguments.  Use it to create flows
  /// that do not need to propagate dataflow information.
   template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeStmtAction,
      typename AfterStmtAction,
      typename BetweenStmtAction,
      typename BeforeExprAction,
      typename AfterExprAction,
     typename ExprFlow>
   typename ptr<ForwardFlow<EnterAction, LeaveAction, BeforeStmtAction, AfterStmtAction,
	       BetweenStmtAction, BeforeExprAction, AfterExprAction, ExprFlow,
			    NullDataflow, NullDataflow::Confluence> >::type
   makeForwardFlow(const EnterAction &ent, const LeaveAction &lv,
		   const BeforeStmtAction &bs,
		   const AfterStmtAction &as, const BetweenStmtAction &bts,
		   const BeforeExprAction &be, const AfterExprAction &ae,
		   const ExprFlow &ef) {
     typedef ForwardFlow<EnterAction, LeaveAction, BeforeStmtAction, AfterStmtAction,
       BetweenStmtAction, BeforeExprAction, AfterExprAction, ExprFlow,
       NullDataflow, NullDataflow::Confluence> flow_type;
     return typename ptr<flow_type>::type(new flow_type(ent, lv, bs, as, bts, be, ae, ef,
							NullDataflow(), NullDataflow::Confluence()));
   }
}

#endif
