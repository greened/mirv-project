#ifndef MIRV_Filter_ForwardFlow_hh
#define MIRV_Filter_ForwardFlow_hh

#include <mirv/filter/flow.hh>

namespace MIRV {
   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename ExprFlow = NullExpressionFlow,
      typename Dataflow = NullDataflow,
      typename Confluence = Dataflow::Confluence>
   class ForwardFlow
         : public StatementFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      ExprFlow,
      Dataflow,
      Confluence> {
   public:
      ForwardFlow(EnterAction &e,
                  LeaveAction &l,
                  BeforeAction &b,
                  AfterAction &a,
                  ExprFlow &expr,
                  Dataflow &d,
                  Confluence &c)
            : StatementFlow(e, l, b, a, expr, d, c) {}

      void visit(Statement<Block> &stmt) {
         enter(stmt);
         for(Statement<Block>::iterator s = stmt.begin(),
                send = stmt.end();
             s != send;
             ;) {
            before_statement(stmt, *s);
            (*s)->accept(*this);
            after_statement(stmt, **s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               between(stmt, **prev, **s);
            }
         }
         leave(stmt);
      }

      void visit(Statement<IfThen> &stmt) {
         bool hit_break = has_break();
         enter(stmt);

         before_expression(stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(stmt, *stmt.expression());

         Dataflow denter(dataflow());

         has_break() = false;

         before_statement(stmt, *stmt.child());
         stmt.child()->accept(*this);
         after_statement(stmt, *stmt.child());

         if (!has_break()) {
            confluence(dataflow(), dataflow(), denter);
         }

         leave(stmt);
         has_break() = hit_break;
      }

      void visit(Statement<IfElse> &stmt) {
         bool hit_break = has_break();
         enter()(stmt);

         before_expression(stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(stmt, *stmt.expression());

         Dataflow denter(dataflow());

         has_break() = false;
         Statement<IfElse>::iterator s = stmt.begin();

         before_statement(stmt, **s);
         (*s)->accept(*this);
         after_statement(stmt, **s);

         Dataflow then(dataflow());

         bool then_break = has_break();
         has_break() = false;

         Statement<IfElse>::iterator prev = s++;

         between_statement(stmt, **prev, **s);

         dataflow() = enter;

         before_statement(stmt, **s);
         (*s)->accept(*this);
         after_statement(stmt, **s);

         if (has_break()) {
            if (!then_break) {
               dataflow() = then;
            }
            else {
               dataflow() = denter;
            }
         }
         else if (!then_break) {
            confluence(dataflow(), dataflow(), then);
         }
         // Otherwise we pass through the else clause and don't need
         // to confluence.
         leave(stmt);
         has_break() = hit_break;
      }

      void visit(Statement<While> &stmt) {
         bool hit_break = has_break();
         enter(stmt);

         Dataflow denter(dataflow());

         before_expression(stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(stmt, *stmt.expression());

         Dataflow first_expr(dataflow());

         do {
            before_statement(stmt, *stmt.child());
            stmt.child()->accept(*this);
            after_statement(stmt, *stmt.child());

            if (!has_break()) {
               confluence(dataflow(), dataflow(), denter);
            }
            else {
               dataflow() = denter;
               break;
            }

            before_expression(stmt, *stmt.expression());
            stmt.expression()->accept(expression());
            after_expression(stmt, *stmt.expression());
         } while (dataflow().change() && !has_break());

         if (!has_break()) {
            // Iterating vs. never enter
            confluence(dataflow(), dataflow(), first_expr);
         }

         leave(stmt);
         has_break() = hit_break;
      }

      void visit(Statement<DoWhile> &stmt) {
         bool hit_break = has_break();
         enter(stmt);

         Dataflow denter(dataflow());

         do {
            before_statement(stmt, *stmt.child());
            stmt.child()->accept(*this);
            after_statement(stmt, *stmt.child());

            before_expression(stmt, *stmt.expression());
            stmt.expression()->accept(expression());
            after_expression(stmt, *stmt.expression());

            if (!has_break()) {
               confluence(dataflow(), dataflow(), denter);
            }
            else {
               dataflow() = denter;
               break;
            }
         } while (dataflow().change() && !has_break());

         // Always at least one iteration so no need for confluence
         // here

         leave(stmt);
         has_break() = hit_break;
      }

      void visit(Statement<Switch> &stmt) {
         bool hit_break = has_break();
         has_break() = false;

         enter(stmt);

         before_expression(stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(stmt, *stmt.expression());

         Dataflow denter(dataflow());

         for(Statement<Switch>::iterator s = stmt.begin(),
                send = stmt.end();
             s != send;
             ;) {
            before_statement(stmt, *s);
            (*s)->accept(*this);
            after_statement(stmt, **s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               between_statement(stmt, **prev, **s);

               if (has_break()) {
                  // Don't propagate dataflow from this block to the next
                  // one
                  dataflow() = denter;
                  has_break() = false;
                  // Exit confluence handled at after statement
               }
               else {
                  // Fall-through vs. direct jump
                  confluence(dataflow(), dataflow(), denter);
               }
            }
         }

         leave(stmt);
         has_break() = hit_break;
      }

      void visit(Statement<Case> &stmt) {
         enter(stmt);

         before_expression(stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(stmt, *stmt.expression());

         before_statement(stmt, *stmt.child());
         stmt.child()->accept(*this);
         after_statement(stmt, *stmt.child());

         leave(stmt);
      }

      void visit(Statement<CaseBlock> &stmt);

      void visit(Statement<Before> &stmt) {
         enter(stmt);

         Dataflow &label = get_attribute<Dataflow>(stmt);

         do {
            confluence(dataflow(), dataflow(), label);

            before_statement(stmt, *stmt.child());
            stmt.child()->accept(*this);
            after_statement(stmt, *stmt.child());
         } while(label.changed() || dataflow.changed());

         leave(stmt);
      }

      void visit(Statement<After> &stmt) {
         enter(stmt);

         before_statement(stmt, *stmt.child());
         stmt.child()->accept(*this);
         after_statement(stmt, *stmt.child());

         Dataflow &label = get_attribute<Dataflow>(stmt);

         confluence(dataflow(), dataflow(), label);

         leave(stmt);
      }

      void visit(Statement<Goto> &stmt) {
         enter(stmt);

         Dataflow &label = get_attribute<Dataflow>(*stmt.target());
         confluence(label, label, dataflow());

         leave(stmt);
      }

      void visit(Statement<Return> &stmt) {
         enter(stmt);
         leave(stmt);
      }
   };
}

#endif
