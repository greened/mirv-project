#ifndef mirv_core_filter_statement_flow_hh
#define mirv_core_filter_statement_flow_hh

#include <mirv/core/filter/statement_visitor.hh>
#include <mirv/core/filter/action.hh>
#include <mirv/core/filter/expression_flow.hh>
#include <mirv/core/filter/dataflow.hh>
#include <mirv/core/mem/heap.hh>

namespace mirv {
  /// This is the base class of all flows that walk statement trees.
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
     /// The action to apply upon entering a statement.
      EnterAction ent;
     /// The action to apply upon leaving a statement.
      LeaveAction lve;
     /// The action to apply before visiting a child statement.
     BeforeStmtAction bfrstmt;
     /// The action to apply after visiting a child statement.
      AfterStmtAction aftstmt;
     /// The action to apply between a pair of child statements.
      BetweenStmtAction betstmt;
     /// The action to apply before visiting a child expression.
       BeforeExprAction bfrexpr;
     /// The action to apply after visiting a child expression.
       AfterExprAction aftexpr;
     /// The flow to walk child expression trees.
     ExprFlow exp;
    /// The dataflow information to propagate. 
     Dataflow data;
     /// The dataflow confluence operation.
     Confluence conf;

     bool hit_break;  /// Have we seen a break statement?

   protected:
     /// Apply the enter action.
     template<typename Stmt>
     typename EnterAction::result_type enter(boost::shared_ptr<Stmt> stmt) {
       return(ent(stmt));
      }

     /// Apply the leave action.
     template<typename Stmt>
     typename LeaveAction::result_type leave(boost::shared_ptr<Stmt> stmt) {
       return(lve(stmt));
      }

     /// Apply the before action.
     template<typename Stmt, typename Child>
     typename BeforeStmtAction::result_type before_statement(boost::shared_ptr<Stmt> stmt,
							     boost::shared_ptr<Child> child) {
       return(bfrstmt(stmt, child));
      }

     /// Apply the after action.
     template<typename Stmt, typename Child>
     typename AfterStmtAction::result_type after_statement(boost::shared_ptr<Stmt> stmt,
							   boost::shared_ptr<Child> child) {
       return(aftstmt(stmt, child));
      }

     /// Apply the between action.
     template<typename Stmt, typename Child>
     typename BetweenStmtAction::result_type between_statement(boost::shared_ptr<Stmt> stmt,
							       boost::shared_ptr<Child> child1,
							       boost::shared_ptr<Child> child2) {
       return(betstmt(stmt, child1, child2));
      }

     /// Apply the before expression action.
     template<typename Stmt, typename Expr>
     typename BeforeExprAction::result_type before_expression(boost::shared_ptr<Stmt> stmt,
							      boost::shared_ptr<Expr> expr) {
       return(bfrexpr(stmt, expr));
      }

     /// Apply the after expression action.
     template<typename Stmt, typename Expr>
     typename AfterExprAction::result_type after_expression(boost::shared_ptr<Stmt> stmt,
							    boost::shared_ptr<Expr> expr) {
       return(aftexpr(stmt, expr));
      }

     /// Get the expression flow.
     ExprFlow &expression_flow(void) {
       return exp;
      }

     // Get the current dataflow information.
      Dataflow &dataflow(void) {
         return(data);
      }

     /// Apply the dataflow confluence.
     typename Confluence::result_type confluence(Dataflow &out,
						  const Dataflow &in1, const Dataflow &in2) {
       return conf(out, in1, in2);
     }

     /// Return whether we saw a break in a child tree.
     bool has_break(void) const {
       return hit_break;
     }

     /// Clear the hit_break flag.
     void set_no_break(void) {
       hit_break = false;
     }
     /// Set the hit_break flag.
     void set_has_break(void) {
       hit_break = true;
     }
     /// Set the hit_break flag to a given value.
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
