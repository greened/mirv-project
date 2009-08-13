#ifndef MIRV_Filter_Flow_hh
#define MIRV_Filter_Flow_hh

#include <mirv/filter/visitor.hh>

namespace MIRV {
   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename ExpressionFlow = NullFlow,
      typename Dataflow = NullDataflow,
      typename Confluence = Dataflow::Confluence>
   class StatementFlow
         : public StatementVisitor {
   private:
      EnterAction ent;
      LeaveAction lve;
      BeforeAction bfr;
      AfterAction aft;
      ExpressionFlow exp;
      Dataflow data;
      Confluence conf;

   protected:
      EnterAction &enter(void) {
         return(ent);
      };

      EnterAction &leave(void) {
         return(lve);
      };

      EnterAction &before(void) {
         return(bfr);
      };

      EnterAction &after(void) {
         return(aft);
      };

      EnterAction &expression(void) {
         return(exp);
      };

      Dataflow &dataflow(void) {
         return(data);
      }
      Confluence &confluence(void) {
         return(conf);
      }

   public:
      StatementFlow(EnterAction &e,
                    LeaveAction &l,
                    BeforeAction &b,
                    AfterAction &a,
                    ExpressionFlow &expr,
                    Dataflow &d,
                    Confluence &c)
            : ent(e), lve(l), bfr(b), aft(a), exp(expr),
                 data(d), conf(c) {}
   };

   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename ExpressionFlow = NullFlow,
      typename Dataflow = NullDataflow,
      typename Confluence = Dataflow::Confluence>
   class ForwardFlow
         : public StatementFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      ExpressionFlow,
      Dataflow,
      Confluence> {
   public:
      ForwardFlow(EnterAction &e,
                  LeaveAction &l,
                  BeforeAction &b,
                  AfterAction &a,
                  ExpressionFlow &expr,
                  Dataflow &d,
                  Confluence &c)
            : StatementFlow(e, l, b, a, expr, d, c) {}

      void visit(Statement<Block> &stmt) {
         enter()(*stmt);
         for(Statement<Block>::iterator s = stmt.begin(),
                send = stmt.end();
             s != send;
             ;) {
            before_statement()(*stmt, *s);
            (*s)->accept(*this);
            after_statement()(*stmt, **s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               between_statement()(*stmt, **prev, **s);
            }
         }
         leave()(*stmt);
      }

      void visit(Statement<IfThen> &stmt) {
         bool hit_break = has_break();
         enter()(*stmt);

         before_expression(*stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(*stmt, *stmt.expression());

         Dataflow denter(dataflow());

         has_break() = false;

         before_statement(*stmt, *stmt.child());
         stmt.child()->accept(*this);
         after_statement(*stmt, *stmt.child());

         if (!has_break()) {
            confluence()(dataflow(), dataflow(), denter);
         }

         leave()(*stmt);
         has_break() = hit_break;
      }

      void visit(Statement<IfElse> &stmt) {
         bool hit_break = has_break();
         enter()(*stmt);

         before_expression(*stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(*stmt, *stmt.expression());

         Dataflow denter(dataflow());

         has_break() = false;
         Statement<IfElse>::iterator s = stmt.begin();

         before_statement(*stmt, **s);
         (*s)->accept(*this);
         after_statement(*stmt, **s);

         Dataflow then(dataflow());

         bool then_break = has_break();
         has_break() = false;

         Statement<IfElse>::iterator prev = s++;

         between_statement(*stmt, **prev, **s);

         dataflow() = enter;

         before_statement(*stmt, **s);
         (*s)->accept(*this);
         after_statement(*stmt, **s);

         if (has_break()) {
            if (!then_break) {
               dataflow() = then;
            }
            else {
               dataflow() = denter;
            }
         }
         else if (!then_break) {
            confluence()(dataflow(), dataflow(), denter);
         }
         // Otherwise we pass through the else clause and don't need
         // to confluence.
         leave()(*stmt);
         has_break() = hit_break;
      }

      void visit(Statement<While> &stmt) {
         bool hit_break = has_break();
         enter()(*stmt);

         Dataflow denter(dataflow());

         before_expression(*stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(*stmt, *stmt.expression());

         Dataflow first_expr(dataflow());

         do {
            before_statement(*stmt, *stmt.child());
            stmt.child()->accept(*this);
            after_statement(*stmt, *stmt.child());

            before_expression(*stmt, *stmt.expression());
            stmt.expression()->accept(expression());
            after_expression(*stmt, *stmt.expression());

            if (!has_break()) {
               if (dataflow().change()) {
                  // Loop back around
                  confluence()(dataflow(), dataflow(), denter);
               }
            }
            else {
               dataflow() = denter;
            }
         } while (dataflow().change() && !has_break());

         if (!has_break()) {
            // Iterating vs. never enter
            confluence()(dataflow(), dataflow(), first_expr);
         }

         leave()(*stmt);
         has_break() = hit_break;
      }

      void visit(Statement<DoWhile> &stmt) {
         bool hit_break = has_break();
         enter()(*stmt);

         Dataflow denter(dataflow());

         do {
            before_statement(*stmt, *stmt.child());
            stmt.child()->accept(*this);
            after_statement(*stmt, *stmt.child());

            before_expression(*stmt, *stmt.expression());
            stmt.expression()->accept(expression());
            after_expression(*stmt, *stmt.expression());

            if (!has_break()) {
               if (dataflow().change()) {
                  // Loop back around
                  confluence()(dataflow(), dataflow(), denter);
               }
            }
            else {
               dataflow() = denter;
            }
         } while (dataflow().change() && !has_break());

         // Always at least one iteration so no need for confluence
         // here

         leave()(*stmt);
         has_break() = hit_break;
      }

      void visit(Statement<Switch> &stmt) {
         bool hit_break = has_break();
         has_break() = false;

         Dataflow denter(dataflow());

         enter()(*stmt);

         for(Statement<Switch>::iterator s = stmt.begin(),
                send = stmt.end();
             s != send;
             ;) {
            before_statement()(*stmt, *s);
            (*s)->accept(*this);
            after_statement()(*stmt, **s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               between_statement()(*stmt, **prev, **s);

               if (has_break()) {
                  // Don't propagate dataflow from this block to the next
                  // one
                  dataflow() = denter;
                  has_break() = false;
                  // Exit confluence handled at after statement
               }
               else {
                  // Fall-through vs. direct jump
                  confluence()(dataflow(), dataflow(), denter);
               }
            }
         }

         leave()(*stmt);
         has_break() = hit_break;
      }

      void visit(Statement<Case> &stmt) {
         enter()(*stmt);

         before_expression(*stmt, *stmt.expression());
         stmt.expression()->accept(expression());
         after_expression(*stmt, *stmt.expression());

         before_statement(*stmt, *stmt.child());
         stmt.child()->accept(*this);
         after_statement(*stmt, *stmt.child());

         leave()(stmt);
      }

      void visit(Statement<CaseBlock> &stmt);

      void visit(Statement<Before> &stmt) {
         enter()(*stmt);

         Dataflow &label = get_attribute<Dataflow>(stmt);

         do {
            confluence()(dataflow(), dataflow(), label);

            before_statement(*stmt, *stmt.child());
            stmt.child()->accept(*this);
            after_statement(*stmt, *stmt.child());
         } while(label.changed());

         leave()(stmt);
      }

      void visit(Statement<After> &stmt) {
         enter()(*stmt);

         before_statement(*stmt, *stmt.child());
         stmt.child()->accept(*this);
         after_statement(*stmt, *stmt.child());

         confluence()(dataflow(), dataflow(), label);

         leave()(stmt);
      }

      void visit(Statement<Goto> &stmt);
      void visit(Statement<Return> &stmt);
   };

   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename ExpressionFlow = NullFlow,
      typename Dataflow = NullDataflow,
      typename Confluence = Dataflow::Confluence>
   class BackwardFlow
         : public StatementFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      ExpressionFlow,
      Dataflow,
      Confluence> {
   public:
      BackwardFlow(EnterAction &e,
                   LeaveAction &l,
                   BeforeAction &b,
                   AfterAction &a,
                   ExpressionFlow &expr,
                   Dataflow &d,
                   Confluence &c)
            : StatementFlow(e, l, b, a, expr, d, c) {}
      void visit(Statement<Block> &stmt);
      void visit(Statement<IfThen> &stmt);
      void visit(Statement<IfElse> &stmt);
      void visit(Statement<While> &stmt);
      void visit(Statement<DoWhile> &stmt);
      void visit(Statement<Switch> &stmt);
      void visit(Statement<Case> &stmt);
      void visit(Statement<CaseBlock> &stmt);
      void visit(Statement<Before> &stmt);
      void visit(Statement<After> &stmt);
      void visit(Statement<Goto> &stmt);
      void visit(Statement<Return> &stmt);
   };

   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename Dataflow = NullDataflow>
   class ExpressionFlow
         : public ExpressionVisitor {
   private:
      EnterAction ent;
      LeaveAction lve;
      BeforeAction bfr;
      AfterAction aft;
      Dataflow data;
      Confluence conf;

   protected:
      EnterAction &enter(void) {
         return(ent);
      };

      EnterAction &leave(void) {
         return(lve);
      };

      EnterAction &before(void) {
         return(bfr);
      };

      EnterAction &after(void) {
         return(aft);
      };

      Dataflow &dataflow(void) {
         return(data);
      }
      Confluence &confluence(void) {
         return(conf);
      }

   public:
      ExpressionFlow(EnterAction &e,
                     LeaveAction &l,
                     BeforeAction &b,
                     AfterAction &a,
                     Dataflow &d,
                     Confluence &c)
            : ent(e), lve(l), bfr(b), aft(a),
                 data(d), conf(c) {}
   };


   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename Dataflow = NullDataflow,
      typename Confluence = Dataflow::Confluence>
   class ForwardExpressionFlow
         : public ExpressionFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      Dataflow,
      Confluence> {
   public:
      ForwardExpressionFlow(EnterAction &e,
                            LeaveAction &l,
                            BeforeAction &b,
                            AfterAction &a,
                            Dataflow &d,
                            Confluence &c)
            : ExpressionFlow(e, l, b, a, expr, d, c) {}

      void visit(Expression<Add> &expr);
      void visit(Expression<Sub> &expr);
      void visit(Expression<Mult> &expr);
      void visit(Expression<Div> &expr);
      void visit(Expression<Mod> &expr);
      void visit(Expression<Neg> &expr);
      void visit(Expression<LogicalAnd> &expr);
      void visit(Expression<LogicalOr> &expr);
      void visit(Expression<LogicalNot> &expr);
      void visit(Expression<BitwiseAnd> &expr);
      void visit(Expression<BitwiseOr> &expr);
      void visit(Expression<Complement> &expr);
   };
}

#endif
