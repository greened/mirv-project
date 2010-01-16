#ifndef mirv_core_filter_statement_flow_hh
#define mirv_core_filter_statement_flow_hh

#include <mirv/core/filter/statement_visitor.hh>
#include <mirv/core/filter/action.hh>
#include <mirv/core/filter/expression_flow.hh>
#include <mirv/core/filter/dataflow.hh>
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
     typename EnterAction::result_type enter(boost::shared_ptr<Stmt> stmt) {
       return(ent(stmt));
      }

     template<typename Stmt>
     typename LeaveAction::result_type leave(boost::shared_ptr<Stmt> stmt) {
       return(lve(stmt));
      }

     template<typename Stmt, typename Child>
     typename BeforeStmtAction::result_type before_statement(boost::shared_ptr<Stmt> stmt,
							     boost::shared_ptr<Child> child) {
       return(bfrstmt(stmt, child));
      }

     template<typename Stmt, typename Child>
     typename AfterStmtAction::result_type after_statement(boost::shared_ptr<Stmt> stmt,
							   boost::shared_ptr<Child> child) {
       return(aftstmt(stmt, child));
      }

     template<typename Stmt, typename Child>
     typename BetweenStmtAction::result_type between_statement(boost::shared_ptr<Stmt> stmt,
							       boost::shared_ptr<Child> child1,
							       boost::shared_ptr<Child> child2) {
       return(betstmt(stmt, child1, child2));
      }

     template<typename Stmt, typename Expr>
     typename BeforeExprAction::result_type before_expression(boost::shared_ptr<Stmt> stmt,
							      boost::shared_ptr<Expr> expr) {
       return(bfrexpr(stmt, expr));
      }

     template<typename Stmt, typename Expr>
     typename AfterExprAction::result_type after_expression(boost::shared_ptr<Stmt> stmt,
							    boost::shared_ptr<Expr> expr) {
       return(aftexpr(stmt, expr));
      }

     ExprFlow &expression_flow(void) {
       return exp;
      }

      Dataflow &dataflow(void) {
         return(data);
      }
     typename Confluence::result_type confluence(Dataflow &out,
						  const Dataflow &in1, const Dataflow &in2) {
       return conf(out, in1, in2);
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
     StatementFlow(const EnterAction &e = NullAction(),
		   const LeaveAction &l = NullAction(),
		   const BeforeStmtAction &bs = NullAction(),
		   const AfterStmtAction &as = NullAction(), 
		   const BetweenStmtAction &bt = NullAction(),
		   const BeforeExprAction &be = NullAction(),
		   const AfterExprAction &ae = NullAction(), 
		   const ExprFlow &expr = NullExpressionFlow(),
		   const Dataflow &d = NullDataflow(),
		   const Confluence &c = typename Dataflow::Confluence())
	: ent(e), lve(l), bfrstmt(bs), aftstmt(as), betstmt(bt), bfrexpr(be),
	  aftexpr(ae), exp(expr), data(d), conf(c) {}
   };

  typedef StatementFlow<> NullStatementFlow;
}

#endif
