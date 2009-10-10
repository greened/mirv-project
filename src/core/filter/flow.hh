#ifndef MIRV_Filter_Flow_hh
#define MIRV_Filter_Flow_hh

#include <mirv/filter/visitor.hh>
#include <mirv/filter/action.hh>
#include <mirv/filter/expression_flow.hh>
#include <mirv/filter/dataflow.hh>

namespace MIRV {
   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename ExprFlow = NullExpressionFlow,
      typename Dataflow = NullDataflow,
      typename Confluence = Dataflow::Confluence>
   class StatementFlow
     : public StatementVisitor<void>::type {
   private:
      EnterAction ent;
      LeaveAction lve;
      BeforeAction bfr;
      AfterAction aft;
      ExprFlow exp;
      Dataflow data;
      Confluence conf;

   protected:
     template<typename Stmt>
     typename EnterAction::result_type enter(Stmt &stmt) {
       return(ent(stmt));
      };

     template<typename Stmt>
     typename LeaveAction::result_type leave(Stmt &stmt) {
       return(lve(stmt));
      };

     template<typename Stmt>
     typename BeforeAction::result_type before(Stmt &stmt) {
       return(bfr(stmt));
      };

     template<typename Stmt>
     typename AfterAction::result_type after(Stmt &stmt) {
       return(aft(stmt));
      };

     template<typename Expr>
     void expression(Expr &expr) {
       exp(expr);
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
                    ExprFlow &expr,
                    Dataflow &d,
                    Confluence &c)
            : ent(e), lve(l), bfr(b), aft(a), exp(expr),
                 data(d), conf(c) {}
   };
}

#endif
