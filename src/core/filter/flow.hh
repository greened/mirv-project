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
      typename BetweenStmtAction = NullAction,
     typename AfterStmtAction = NullAction,
     typename BeforeExprAction = NullAction,
      typename AfterExprAction = NullAction,
      typename ExprFlow = NullExpressionFlow,
      typename Dataflow = NullDataflow,
      typename Confluence = typename Dataflow::Confluence>
   class StatementFlow : public StatementVisitor {
   private:
      EnterAction ent;
      LeaveAction lve;
      BeforeStmtAction bfrstmt;
      AfterStmtAction aftstmt;
      BetweenStmtAction betstmt;
      BeforeExprAction bfrexpr;
      AfterExprAction aftexpr;
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

     template<typename Stmt>
     typename AfterStmtAction::result_type after_statement(Stmt &stmt) {
       return(aftstmt(stmt));
      };

     template<typename Stmt>
     typename BetweenStmtAction::result_type between_statement(Stmt &stmt) {
       return(betstmt(stmt));
      };

     template<typename Expr>
     typename BeforeExprAction::result_type before_expression(Expr &expr) {
       return(bfrexpr(expr));
      };

     template<typename Expr>
     typename AfterExprAction::result_type after_expression(Expr &expr) {
       return(aftexpr(expr));
      };

     ExprFlow &expression_flow(void) {
       return exp;
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
                    AfterStmtAction &as, 
		    BetweenStmtAction &bt,
                    BeforeExprAction &be,
                    AfterExprAction &ae, 
		    ExprFlow &expr,
                    Dataflow &d,
                    Confluence &c)
	: ent(e), lve(l), bfrstmt(bs), aftstmt(as), betstmt(bt), bfrexpr(be),
	  aftexpr(ae), exp(expr), data(d), conf(c) {}
   };
}

#endif
