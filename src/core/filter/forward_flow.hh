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

     void visit(ptr<Statement<Block> >::type stmt) {
         this->enter(stmt);
         for(Statement<Block>::iterator s = stmt->begin(),
                send = stmt->end();
             s != send;
             /* NULL */) {
            this->before_statement(stmt, *s);
            (*s)->accept(*this);
            this->after_statement(stmt, *s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               this->between_statement(stmt, *prev, *s);
            }
         }
         this->leave(stmt);
      }

      void visit(ptr<Statement<IfThen> >::type stmt) {
         bool hit_break = this->has_break();
         this->enter(stmt);

         this->before_expression(stmt, stmt->get_expression());
         stmt->get_expression()->accept(this->expression_flow());
         this->after_expression(stmt, stmt->get_expression());

         Dataflow denter(this->dataflow());

         this->set_no_break();

         this->before_statement(stmt, stmt->get_child_statement());
         stmt->get_child_statement()->accept(*this);
         this->after_statement(stmt, stmt->get_child_statement());

         if (!this->has_break()) {
            confluence(this->dataflow(), this->dataflow(), denter);
         }

         this->leave(stmt);
         this->set_break(hit_break);
      }

      void visit(ptr<Statement<IfElse> >::type stmt) {
         bool hit_break = this->has_break();
         this->enter(stmt);

         this->before_expression(stmt, stmt->get_expression());
         stmt->get_expression()->accept(this->expression_flow());
         this->after_expression(stmt, stmt->get_expression());

         Dataflow denter(this->dataflow());

         this->set_no_break();
         Statement<IfElse>::iterator s = stmt->begin();

         this->before_statement(stmt, *s);
         (*s)->accept(*this);
         this->after_statement(stmt, *s);

         Dataflow then(this->dataflow());

         bool then_break = this->has_break();
         this->set_no_break();

         Statement<IfElse>::iterator prev = s++;

         this->between_statement(stmt, *prev, *s);

         this->dataflow() = denter;

         this->before_statement(stmt, *s);
         (*s)->accept(*this);
         this->after_statement(stmt, *s);

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
         this->set_break(hit_break);
      }

      void visit(ptr<Statement<While> >::type stmt) {
         bool hit_break = this->has_break();
         this->enter(stmt);

         Dataflow denter(this->dataflow());

         this->before_expression(stmt, stmt->get_expression());
         stmt->get_expression()->accept(this->expression_flow());
         this->after_expression(stmt, stmt->get_expression());

         Dataflow first_expr(this->dataflow());

         do {
            this->before_statement(stmt, stmt->get_child_statement());
            stmt->get_child_statement()->accept(*this);
            this->after_statement(stmt, stmt->get_child_statement());

            if (!this->has_break()) {
               confluence(this->dataflow(), this->dataflow(), denter);
            }
            else {
               this->dataflow() = denter;
               break;
            }

            this->before_expression(stmt, stmt->get_expression());
            stmt->get_expression()->accept(this->expression_flow());
            this->after_expression(stmt, stmt->get_expression());
         } while (this->dataflow().change() && !this->has_break());

         if (!this->has_break()) {
            // Iterating vs. never enter
            confluence(this->dataflow(), this->dataflow(), first_expr);
         }

         this->leave(stmt);
         this->set_break(hit_break);
      }

      void visit(ptr<Statement<DoWhile> >::type stmt) {
         bool hit_break = this->has_break();
         this->enter(stmt);

         Dataflow denter(this->dataflow());

         do {
            this->before_statement(stmt, stmt->get_child_statement());
            stmt->get_child_statement()->accept(*this);
            this->after_statement(stmt, stmt->get_child_statement());

            this->before_expression(stmt, stmt->get_expression());
            stmt->get_expression()->accept(this->expression_flow());
            this->after_expression(stmt, stmt->get_expression());

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
         this->set_break(hit_break);
      }

      void visit(ptr<Statement<Switch> >::type stmt) {
         bool hit_break = this->has_break();
         this->set_no_break();

         this->enter(stmt);

         this->before_expression(stmt, stmt->get_expression());
         stmt->get_expression()->accept(this->expression_flow());
         this->after_expression(stmt, stmt->get_expression());

         Dataflow denter(this->dataflow());

         for(Statement<Switch>::iterator s = stmt->begin(),
                send = stmt->end();
             s != send;
             /* NULL */) {
            this->before_statement(stmt, *s);
            (*s)->accept(*this);
            this->after_statement(stmt, *s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               this->between_statement(stmt, *prev, *s);

               if (this->has_break()) {
                  // Don't propagate dataflow from this block to the next
                  // one
                  this->dataflow() = denter;
                  this->set_no_break();
                  // Exit confluence handled at after statement
               }
               else {
                  // Fall-through vs. direct jump
                  confluence(this->dataflow(), this->dataflow(), denter);
               }
            }
         }

         this->leave(stmt);
         this->set_break(hit_break);
      }

      void visit(ptr<Statement<Case> >::type stmt) {
         this->enter(stmt);

         this->before_expression(stmt, stmt->get_expression());
         stmt->get_expression()->accept(this->expression_flow());
         this->after_expression(stmt, stmt->get_expression());

         this->before_statement(stmt, stmt->get_child_statement());
         stmt->get_child_statement()->accept(*this);
         this->after_statement(stmt, stmt->get_child_statement());

         this->leave(stmt);
      }

     void visit(ptr<Statement<CaseBlock> >::type stmt) {
     }

      void visit(ptr<Statement<Before> >::type stmt) {
         this->enter(stmt);

         typename ptr<Dataflow>::type label = get_attribute<Dataflow>(stmt);

         do {
            confluence(this->dataflow(), this->dataflow(), *label);

            this->before_statement(stmt, stmt->get_child_statement());
            stmt->get_child_statement()->accept(*this);
            this->after_statement(stmt, stmt->get_child_statement());
         } while(label->change() || this->dataflow().change());

         this->leave(stmt);
      }

      void visit(ptr<Statement<After> >::type stmt) {
         this->enter(stmt);

         this->before_statement(stmt, stmt->get_child_statement());
         stmt->get_child_statement()->accept(*this);
         this->after_statement(stmt, stmt->get_child_statement());

         typename ptr<Dataflow>::type label = get_attribute<Dataflow>(stmt);

         confluence(this->dataflow(), this->dataflow(), *label);

         this->leave(stmt);
      }

      void visit(ptr<Statement<Goto> >::type stmt) {
         this->enter(stmt);

         typename ptr<Dataflow>::type label = get_attribute<Dataflow>(stmt->get_label());
         confluence(*label, *label, this->dataflow());

         this->leave(stmt);
      }

      void visit(ptr<Statement<Return> >::type stmt) {
         this->enter(stmt);
         this->leave(stmt);
      }

      void visit(ptr<Statement<Assignment> >::type stmt) {
         this->enter(stmt);

         this->before_expression(stmt, stmt->get_right_expression());
         stmt->get_right_expression()->accept(this->expression_flow());
         this->after_expression(stmt, stmt->get_right_expression());

         this->before_expression(stmt, stmt->get_left_expression());
         stmt->get_left_expression()->accept(this->expression_flow());
         this->after_expression(stmt, stmt->get_left_expression());

         this->leave(stmt);
      }
   };

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
   make_forward_flow(const EnterAction &ent, const LeaveAction &lv,
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
   make_forward_flow(const EnterAction &ent, const LeaveAction &lv,
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
