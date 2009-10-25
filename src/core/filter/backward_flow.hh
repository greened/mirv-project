#ifndef mirv_core_filter_backward_flow_hh
#define mirv_core_filter_backward_flow_hh

#include <mirv/filter/flow.hh>

namespace mirv {
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

      void visit(Statement<Block> &stmt) {
         enter(stmt);
         for(Statement<Block>::reverse_iterator s = stmt.rbegin(),
                send = stmt.rend();
             s != send;
             ;) {
            before_statement(stmt, *s);
            (*s)->accept(*this);
            after_statement(stmt, **s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               between_statement(stmt, **prev, **s);
            }
         }
         leave(stmt);
      }

      void visit(Statement<IfThen> &stmt) {
         enter(stmt);

         Dataflow denter(dataflow());

         before_statement(stmt, *stmt.child());
         stmt.child()->accept(*this);
         after_statement(stmt, *stmt.child());

         confluence(dataflow(), dataflow(), denter);

         before_expression(stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(stmt, *stmt.expression());

         leave(stmt);
      }

      void visit(Statement<IfElse> &stmt) {
         enter(stmt);

         Dataflow denter(dataflow());

         Statement<IfElse>::iterator s = stmt.begin();

         before_statement(stmt, **s);
         (*s)->accept(*this);
         after_statement(stmt, **s);

         Dataflow then(dataflow());

         Statement<IfElse>::iterator prev = s++;

         between_statement(stmt, **prev, **s);

         dataflow() = denter;

         before_statement(stmt, **s);
         (*s)->accept(*this);
         after_statement(stmt, **s);

         confluence(dataflow(), dataflow(), then);

         before_expression(stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(stmt, *stmt.expression());

         leave(stmt);
      }

      void visit(Statement<While> &stmt) {
         enter(stmt);

         before_expression(stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(stmt, *stmt.expression());

         Dataflow first_expr(dataflow());

         do {
            before_statement(stmt, *stmt.child());
            stmt.child()->accept(*this);
            after_statement(stmt, *stmt.child());

            // Is denter right?
            confluence(dataflow(), dataflow(), denter);

            before_expression(stmt, *stmt.expression());
            stmt.expression()->accept(expression());
            after_expression(stmt, *stmt.expression());
         } while (dataflow().change());

         // Iterating vs. never enter
         confluence(dataflow(), dataflow(), first_expr);

         leave(stmt);
      }

      void visit(Statement<DoWhile> &stmt) {
         enter(stmt);

         Dataflow denter(dataflow());

         do {
            before_statement(stmt, *stmt.child());
            stmt.child()->accept(*this);
            after_statement(stmt, *stmt.child());

            before_expression(stmt, *stmt.expression());
            stmt.expression()->accept(expression());
            after_expression(stmt, *stmt.expression());

            confluence(dataflow(), dataflow(), denter);
         } while (dataflow().change());

         // Always at least one iteration so no need for confluence
         // here

         leave(stmt);
      }

      void visit(Statement<Switch> &stmt) {
         enter(stmt);

         Dataflow into_expr;

         for(Statement<Switch>::reverse_iterator s = stmt.rbegin(),
                send = stmt.rend();
             s != send;
             ;) {
            before_statement(stmt, *s);
            (*s)->accept(*this);
            after_statement(stmt, **s);
            Statement<Block>::iterator prev = s;

            // Fall-through vs. direct jump
            confluence(into_expr, into_expr, dataflow());

            if (++s != send) {
               between_statement(stmt, **prev, **s);
            }
         }

         dataflow() = into_expr;

         before_expression(stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(stmt, *stmt.expression());

         leave(stmt);
      }

      void visit(Statement<Case> &stmt) {
         enter(stmt);

         before_statement(stmt, *stmt.child());
         stmt.child()->accept(*this);
         after_statement(stmt, *stmt.child());

         before_expression(stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(stmt, *stmt.expression());

         leave(stmt);
      }

      void visit(Statement<CaseBlock> &stmt);

      void visit(Statement<Before> &stmt) {
         enter(stmt);

         do {
            before_statement(stmt, *stmt.child());
            stmt.child()->accept(*this);
            after_statement(stmt, *stmt.child());

            Dataflow &label = get_attribute<Dataflow>(stmt);

            confluence(dataflow(), dataflow(), label);
         } while(label.changed() || dataflow.changed());

         leave(stmt);
      }

      void visit(Statement<After> &stmt) {
         enter(stmt);

         Dataflow &label = get_attribute<Dataflow>(stmt);

         confluence(dataflow(), dataflow(), label);

         before_statement(stmt, *stmt.child());
         stmt.child()->accept(*this);
         after_statement(stmt, *stmt.child());

         leave(stmt);
      }

      void visit(Statement<Goto> &stmt) {
         enter(stmt);

         Dataflow &label = get_attribute<Dataflow>(*stmt.target());
         dataflow() = label;  // Pick up data from where I go to

         leave(stmt);
      }

      void visit(Statement<Return> &stmt) {
         enter(stmt);
         leave(stmt);
      }
   };
}

#endif
