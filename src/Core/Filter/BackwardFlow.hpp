#ifndef mirv_Core_Filter_BackwardFlow_hpp
#define mirv_Core_Filter_BackwardFlow_hpp

#include <mirv/Filter/Flow.hpp>

namespace mirv {
  /// This flow walks backward through statements, visiting rightmost
  /// operands first.
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
      typename Confluence = Dataflow::Confluence>
   class BackwardFlow
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
   public:
      BackwardFlow(const EnterAction &e,
                   const LeaveAction &l,
                   const BeforeStmtAction &bs,
                   const AfterStmtAction &as,
                   const BetweenStmtAction &bts,
                   const BeforeExprAction &be,
                   const AfterExprAction &ae,
                   const ExprFlow &expr,
                   const Dataflow &d,
                   const Confluence &c)
	: StatementFlow(e, l, bs, as, bts, be, ae, expr, d, c) {}

     /// Visit block statements from the last statement to the first.
      void visit(ptr<Statement<Block> >::type stmt) {
         enter(stmt);
         for(Statement<Block>::reverse_iterator s = stmt->rbegin(),
                send = stmt->rend();
             s != send;
             ;) {
            beforeStatement(stmt, *s);
            (*s)->accept(*this);
            afterStatement(stmt, **s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               betweenStatement(stmt, **prev, **s);
            }
         }
         leave(stmt);
      }

     /// Visit IfThen statements, visiting the then-statement, then
     /// the controlling expression.
      void visit(ptr<Statement<IfThen> >::type stmt) {
         enter(stmt);

         Dataflow denter(dataflow());

         beforeStatement(stmt, *stmt->child());
         stmt->child()->accept(*this);
         afterStatement(stmt, *stmt->child());

         confluence(dataflow(), dataflow(), denter);

         beforeExpression(stmt, *stmt->expression());
         stmt->expression()->accept(expression());
         afterExpression(stmt, *stmt->expression());

         leave(stmt);
      }

     /// Visit IfElse statements, visiting the then-statement, then
     /// the else-statement, then the controlling expression.
      void visit(ptr<Statement<IfElse> >::type stmt) {
         enter(stmt);

         Dataflow denter(dataflow());

         Statement<IfElse>::iterator s = stmt->begin();

         beforeStatement(stmt, **s);
         (*s)->accept(*this);
         afterStatement(stmt, **s);

         Dataflow then(dataflow());

         Statement<IfElse>::iterator prev = s++;

         betweenStatement(stmt, **prev, **s);

         dataflow() = denter;

         beforeStatement(stmt, **s);
         (*s)->accept(*this);
         afterStatement(stmt, **s);

         confluence(dataflow(), dataflow(), then);

         beforeExpression(stmt, *stmt->expression());
         stmt->expression()->accept(expression());
         afterExpression(stmt, *stmt->expression());

         leave(stmt);
      }

     /// Visit While statements, visiting the body, then the
     /// controlling expression.
      void visit(ptr<Statement<While> >::type stmt) {
         enter(stmt);

         beforeExpression(stmt, *stmt->expression());
         stmt->expression()->accept(expression());
         afterExpression(stmt, *stmt->expression());

         Dataflow firstExpr(dataflow());

         do {
            beforeStatement(stmt, *stmt->child());
            stmt->child()->accept(*this);
            afterStatement(stmt, *stmt->child());

            // Is denter right?
            confluence(dataflow(), dataflow(), denter);

            beforeExpression(stmt, *stmt->expression());
            stmt->expression()->accept(expression());
            afterExpression(stmt, *stmt->expression());
         } while (dataflow().change());

         // Iterating vs-> never enter
         confluence(dataflow(), dataflow(), firstExpr);

         leave(stmt);
      }

     /// Visit DoWhile statements, visiting the controlling
     /// expression, then the body.
      void visit(ptr<Statement<DoWhile> >::type stmt) {
         enter(stmt);

         Dataflow denter(dataflow());

         do {
            beforeStatement(stmt, *stmt->child());
            stmt->child()->accept(*this);
            afterStatement(stmt, *stmt->child());

            beforeExpression(stmt, *stmt->expression());
            stmt->expression()->accept(expression());
            afterExpression(stmt, *stmt->expression());

            confluence(dataflow(), dataflow(), denter);
         } while (dataflow()->change());

         // Always at least one iteration so no need for confluence
         // here

         leave(stmt);
      }

     /// Visit Switch statements, visiting the cases in reverse order,
     /// then the controlling expression.
      void visit(ptr<Statement<Switch> >::type stmt) {
         enter(stmt);

         Dataflow intoExpr;

         for(Statement<Switch>::reverse_iterator s = stmt->rbegin(),
                send = stmt->rend();
             s != send;
             ;) {
            beforeStatement(stmt, *s);
            (*s)->accept(*this);
            afterStatement(stmt, **s);
            Statement<Block>::iterator prev = s;

            // Fall-through vs. direct jump
            confluence(intoExpr, intoExpr, dataflow());

            if (++s != send) {
               betweenStatement(stmt, **prev, **s);
            }
         }

         dataflow() = intoExpr;

         beforeExpression(stmt, *stmt->expression());
         stmt->expression()->accept(expression());
         afterExpression(stmt, *stmt->expression());

         leave(stmt);
      }

     /// Visit Case statements, visiting the body, then the
     /// controlling expression.
      void visit(ptr<Statement<Case> >::type stmt) {
         enter(stmt);

         beforeStatement(stmt, *stmt->child());
         stmt->child()->accept(*this);
         afterStatement(stmt, *stmt->child());

         beforeExpression(stmt, *stmt->expression());
         stmt->expression()->accept(expression());

         afterExpression(stmt, *stmt->expression());

         leave(stmt);
      }

      void visit(ptr<Statement<CaseBlock> >::type stmt);

     /// Visit Before statements.
      void visit(ptr<Statement<Before> >::type stmt) {
         enter(stmt);

         do {
            beforeStatement(stmt, *stmt->child());
            stmt->child()->accept(*this);
            afterStatement(stmt, *stmt->child());

            Dataflow &label = get_attribute<Dataflow>(stmt);

            confluence(dataflow(), dataflow(), label);
         } while(label.changed() || dataflow.changed());

         leave(stmt);
      }

     /// Visit After statements.
      void visit(ptr<Statement<After> >::type stmt) {
         enter(stmt);

         Dataflow &label = get_attribute<Dataflow>(stmt);

         confluence(dataflow(), dataflow(), label);

         beforeStatement(stmt, *stmt->child());
         stmt->child()->accept(*this);
         afterStatement(stmt, *stmt->child());

         leave(stmt);
      }

     /// Visit Goto statements.
      void visit(ptr<Statement<Goto> >::type stmt) {
         enter(stmt);

         Dataflow &label = get_attribute<Dataflow>(*stmt->target());
         dataflow() = label;  // Pick up data from where I go to

         leave(stmt);
      }

     /// Visit Return statements.
      void visit(ptr<Statement<Return> >::type stmt) {
         enter(stmt);
         leave(stmt);
      }

     /// Visit Assignment statements, visiting the left-hand side,
     /// then the right-hand side.
      void visit(ptr<Statement<Assignment> >::type stmt) {
         this->enter(stmt);

         this->beforeExpression(stmt, stmt->getLeftExpression());
         stmt->getLeftExpression()->accept(this->expressionFlow());
         this->afterExpression(stmt, stmt->getLeftExpression());

         this->beforeExpression(stmt, stmt->getRightExpression());
         stmt->getRightExpression()->accept(this->expressionFlow());
         this->afterExpression(stmt, stmt->getRightExpression());

         this->leave(stmt);
      }

   };
}

#endif
