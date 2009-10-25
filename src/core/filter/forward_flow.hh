#ifndef mirv_core_filter_forward_flow_hh
#define mirv_core_filter_forward_flow_hh

#include <mirv/filter/flow.hh>
#include <mirv/filter/dataflow.hh>
#include <mirv/ir/attribute.hh>

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
      BeforeExprAction,
      AfterAction,
      BetweenAction,
      ExprFlow,
      Dataflow,
       Confluence> BaseType;

   public:
      ForwardFlow(EnterAction &e,
                  LeaveAction &l,
                  BeforeStmtAction &bs,
                  BeforeExprAction &be,
                  AfterAction &a,
                  BetweenAction &bt,
                  ExprFlow &expr,
                  Dataflow &d,
                  Confluence &c)
	: BaseType(e, l, bs, be, a, bt, expr, d, c) {}

      void visit(Statement<Block> &stmt) {
         this->enter(stmt);
         for(Statement<Block>::iterator s = stmt.begin(),
                send = stmt.end();
             s != send;
             /* NULL */) {
            this->before_statement(stmt, *s);
            (*s)->accept(*this);
            this->after_statement(stmt, **s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               this->between(stmt, **prev, **s);
            }
         }
         this->leave(stmt);
      }

      void visit(Statement<IfThen> &stmt) {
         bool hit_break = this->has_break();
         this->enter(stmt);

         this->before_expression(stmt, *stmt.get_expression());
         stmt.get_expression()->accept(this->expression_flow());
         this->after_expression(stmt, *stmt.get_expression());

         Dataflow denter(this->dataflow());

         this->has_break() = false;

         this->before_statement(stmt, *stmt.get_child_statement());
         stmt.get_child_statement()->accept(*this);
         this->after_statement(stmt, *stmt.get_child_statement());

         if (!this->has_break()) {
            confluence(this->dataflow(), this->dataflow(), denter);
         }

         this->leave(stmt);
         this->has_break() = hit_break;
      }

      void visit(Statement<IfElse> &stmt) {
         bool hit_break = this->has_break();
         this->enter(stmt);

         this->before_expression(stmt, *stmt.get_expression());
         stmt.get_expression()->accept(this->expression_flow());
         this->after_expression(stmt, *stmt.get_expression());

         Dataflow denter(this->dataflow());

         this->has_break() = false;
         Statement<IfElse>::iterator s = stmt.begin();

         this->before_statement(stmt, **s);
         (*s)->accept(*this);
         this->after_statement(stmt, **s);

         Dataflow then(this->dataflow());

         bool then_break = this->has_break();
         this->has_break() = false;

         Statement<IfElse>::iterator prev = s++;

         this->between_statement(stmt, **prev, **s);

         this->dataflow() = denter;

         this->before_statement(stmt, **s);
         (*s)->accept(*this);
         this->after_statement(stmt, **s);

         if (this->has_break()) {
            if (!then_break) {
               this->dataflow() = then;
            }
            else {
               this->dataflow() = denter;
            }
         }
         else if (!then_break) {
            confluence(this->dataflow(), this->dataflow(), then);
         }
         // Otherwise we pass through the else clause and don't need
         // to confluence.
         this->leave(stmt);
         this->has_break() = hit_break;
      }

      void visit(Statement<While> &stmt) {
         bool hit_break = this->has_break();
         this->enter(stmt);

         Dataflow denter(this->dataflow());

         this->before_expression(stmt, *stmt.get_expression());
         stmt.get_expression()->accept(this->expression_flow());
         this->after_expression(stmt, *stmt.get_expression());

         Dataflow first_expr(this->dataflow());

         do {
            this->before_statement(stmt, *stmt.get_child_statement());
            stmt.get_child_statement()->accept(*this);
            this->after_statement(stmt, *stmt.get_child_statement());

            if (!this->has_break()) {
               confluence(this->dataflow(), this->dataflow(), denter);
            }
            else {
               this->dataflow() = denter;
               break;
            }

            this->before_expression(stmt, *stmt.get_expression());
            stmt.get_expression()->accept(this->expression_flow());
            this->after_expression(stmt, *stmt.get_expression());
         } while (this->dataflow().change() && !this->has_break());

         if (!this->has_break()) {
            // Iterating vs. never enter
            confluence(this->dataflow(), this->dataflow(), first_expr);
         }

         this->leave(stmt);
         this->has_break() = hit_break;
      }

      void visit(Statement<DoWhile> &stmt) {
         bool hit_break = this->has_break();
         this->enter(stmt);

         Dataflow denter(this->dataflow());

         do {
            this->before_statement(stmt, *stmt.get_child_statement());
            stmt.get_child_statement()->accept(*this);
            this->after_statement(stmt, *stmt.get_child_statement());

            this->before_expression(stmt, *stmt.get_expression());
            stmt.get_expression()->accept(this->expression_flow());
            this->after_expression(stmt, *stmt.get_expression());

            if (!this->has_break()) {
               confluence(this->dataflow(), this->dataflow(), denter);
            }
            else {
               this->dataflow() = denter;
               break;
            }
         } while (this->dataflow().change() && !this->has_break());

         // Always at least one iteration so no need for confluence
         // here

         this->leave(stmt);
         this->has_break() = hit_break;
      }

      void visit(Statement<Switch> &stmt) {
         bool hit_break = this->has_break();
         this->has_break() = false;

         this->enter(stmt);

         this->before_expression(stmt, *stmt.get_expression());
         stmt.get_expression()->accept(this->expression_flow());
         this->after_expression(stmt, *stmt.get_expression());

         Dataflow denter(this->dataflow());

         for(Statement<Switch>::iterator s = stmt.begin(),
                send = stmt.end();
             s != send;
             /* NULL */) {
            this->before_statement(stmt, *s);
            (*s)->accept(*this);
            this->after_statement(stmt, **s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               this->between_statement(stmt, **prev, **s);

               if (this->has_break()) {
                  // Don't propagate dataflow from this block to the next
                  // one
                  this->dataflow() = denter;
                  this->has_break() = false;
                  // Exit confluence handled at after statement
               }
               else {
                  // Fall-through vs. direct jump
                  confluence(this->dataflow(), this->dataflow(), denter);
               }
            }
         }

         this->leave(stmt);
         this->has_break() = hit_break;
      }

      void visit(Statement<Case> &stmt) {
         this->enter(stmt);

         this->before_expression(stmt, *stmt.get_expression());
         stmt.get_expression()->accept(this->expression_flow());
         this->after_expression(stmt, *stmt.get_expression());

         this->before_statement(stmt, *stmt.get_child_statement());
         stmt.get_child_statement()->accept(*this);
         this->after_statement(stmt, *stmt.get_child_statement());

         this->leave(stmt);
      }

      void visit(Statement<CaseBlock> &stmt);

      void visit(Statement<Before> &stmt) {
         this->enter(stmt);

         Dataflow &label = get_attribute<Dataflow>(stmt);

         do {
            confluence(this->dataflow(), this->dataflow(), label);

            this->before_statement(stmt, *stmt.get_child_statement());
            stmt.get_child_statement()->accept(*this);
            this->after_statement(stmt, *stmt.get_child_statement());
         } while(label.changed() || this->dataflow().changed());

         this->leave(stmt);
      }

      void visit(Statement<After> &stmt) {
         this->enter(stmt);

         this->before_statement(stmt, *stmt.get_child_statement());
         stmt.get_child_statement()->accept(*this);
         this->after_statement(stmt, *stmt.get_child_statement());

         Dataflow &label = get_attribute<Dataflow>(stmt);

         confluence(this->dataflow(), this->dataflow(), label);

         this->leave(stmt);
      }

      void visit(Statement<Goto> &stmt) {
         this->enter(stmt);

         Dataflow &label = get_attribute<Dataflow>(*stmt.get_label());
         confluence(label, label, this->dataflow());

         this->leave(stmt);
      }

      void visit(Statement<Return> &stmt) {
         this->enter(stmt);
         this->leave(stmt);
      }
   };

   template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeStmtAction,
      typename BeforeExprAction,
      typename AfterAction,
      typename BetweenAction,
      typename ExprFlow,
      typename Dataflow,
      typename Confluence>
   ForwardFlow<EnterAction, LeaveAction, BeforeStmtAction, BeforeExprAction,
	       AfterAction, BetweenAction, ExprFlow, Dataflow, Confluence>
   make_forward_flow(EnterAction &ent, LeaveAction &lv, BeforeStmtAction &bs,
		     BeforeExprAction &be, AfterAction &aft, BetweenAction &bet,
		     ExprFlow &ef, Dataflow &df, Confluence &cf) {
     return ForwardFlow<
     EnterAction, 
       LeaveAction,
       BeforeStmtAction,
       BeforeExprAction,
       AfterAction,
       BetweenAction,
       ExprFlow,
       Dataflow,
       Confluence>(ent, lv, bs, be, aft, bet, ef, df, cf);
   }
}

#endif
