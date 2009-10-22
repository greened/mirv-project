#ifndef mirv_core_filter_flow_hh
#define mirv_core_filter_flow_hh

#include <mirv/filter/visitor.hh>
#include <mirv/filter/action.hh>
#include <mirv/filter/expression_flow.hh>
#include <mirv/filter/dataflow.hh>

namespace mirv {
   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeStmtAction = NullAction,
      typename BeforeExprAction = NullAction,
      typename AfterAction = NullAction,
      typename BetweenAction = NullAction,
      typename ExprFlow = NullExpressionFlow,
      typename Dataflow = NullDataflow,
      typename Confluence = typename Dataflow::Confluence>
   class StatementFlow
     : public StatementVisitor<void>::type {
   private:
      EnterAction ent;
      LeaveAction lve;
      BeforeStmtAction bfrstmt;
      BeforeExprAction bfrexpr;
      AfterAction aft;
      BetweenAction bet;
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
     typename BeforeStmtAction::result_type before_statement(Stmt &stmt) {
       return(bfrstmt(stmt));
      };

     template<typename Expr>
     typename BeforeExprAction::result_type before_expression(Expr &expr) {
       return(bfrexpr(expr));
      };

     template<typename Stmt>
     typename AfterAction::result_type after(Stmt &stmt) {
       return(aft(stmt));
      };

     template<typename Stmt>
     typename BetweenAction::result_type between(Stmt &stmt) {
       return(bet(stmt));
      };

     template<typename Expr>
     void expression(Expr &expr) {
       exp(expr);
      };

      Dataflow &dataflow(void) {
         return(data);
      }
     Confluence &confluence(Dataflow &out,
			    const Dataflow &in1, const Dataflow &in2) {
       conf(out, in1, in2);
      }

   public:
      StatementFlow(EnterAction &e,
                    LeaveAction &l,
                    BeforeStmtAction &bs,
                    BeforeExprAction &be,
                    AfterAction &a, 
                    BetweenAction &bt,
                    ExprFlow &expr,
                    Dataflow &d,
                    Confluence &c)
	: ent(e), lve(l), bfrstmt(bs), bfrexpr(be), aft(a), bet(bt), exp(expr),
	  data(d), conf(c) {}
   };
}

#endif
