#ifndef mirv_core_filter_flow_hh
#define mirv_core_filter_flow_hh

#include <mirv/filter/visitor.hh>
#include <mirv/filter/action.hh>
#include <mirv/filter/expression_flow.hh>
#include <mirv/filter/dataflow.hh>
#include <mirv/mem/heap.hh>

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

     bool hit_break;  // Have we seen a break statement?

   protected:
     template<typename Stmt>
     typename EnterAction::result_type enter(typename ptr<Stmt>::type stmt) {
       return(ent(stmt));
      };

     template<typename Stmt>
     typename LeaveAction::result_type leave(typename ptr<Stmt>::type stmt) {
       return(lve(stmt));
      };

     template<typename Stmt, typename Child>
     typename BeforeStmtAction::result_type before_statement(typename ptr<Stmt>::type stmt,
							     typename ptr<Child>::type child) {
       return(bfrstmt(stmt, child));
      };

     template<typename Stmt, typename Child>
     typename AfterStmtAction::result_type after_statement(typename ptr<Stmt>::type stmt,
							   typename ptr<Child>::type child) {
       return(aftstmt(stmt, child));
      };

     template<typename Stmt, typename Child>
     typename BetweenStmtAction::result_type between_statement(typename ptr<Stmt>::type stmt,
							       typename ptr<Child>::type child1,
							       typename ptr<Child>::type child2) {
       return(betstmt(stmt, child1, child2));
      };

     template<typename Stmt, typename Expr>
     typename BeforeExprAction::result_type before_expression(typename ptr<Stmt>::type stmt,
							      typename ptr<Expr>::type expr) {
       return(bfrexpr(stmt, expr));
      };

     template<typename Stmt, typename Expr>
     typename AfterExprAction::result_type after_expression(typename ptr<Stmt>::type stmt,
							    typename ptr<Expr>::type expr) {
       return(aftexpr(stmt, expr));
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

     bool has_break(void) const {
       return hit_break;
     }

     void set_no_break(void) {
       hit_break = false;
     }
     void set_has_break(void) {
       hit_break = true;
     }
     void set_break(bool v) {
       hit_break = v;
     }

   public:
      StatementFlow(const EnterAction &e,
                    const LeaveAction &l,
                    const BeforeStmtAction &bs,
                    const AfterStmtAction &as, 
		    const BetweenStmtAction &bt,
                    const BeforeExprAction &be,
                    const AfterExprAction &ae, 
		    const ExprFlow &expr,
                    const Dataflow &d,
                    const Confluence &c)
	: ent(e), lve(l), bfrstmt(bs), aftstmt(as), betstmt(bt), bfrexpr(be),
	  aftexpr(ae), exp(expr), data(d), conf(c) {}
   };
}

#endif
