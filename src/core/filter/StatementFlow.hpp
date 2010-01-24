#ifndef mirv_core_filter_StatementFlow_hpp
#define mirv_core_filter_StatementFlow_hpp

#include <mirv/core/filter/StatementVisitor.hpp>
#include <mirv/core/filter/Action.hpp>
#include <mirv/core/filter/ExpressionFlow.hpp>
#include <mirv/core/filter/Dataflow.hpp>
#include <mirv/core/mem/Heap.hpp>

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

     bool hitBreak;  /// Have we seen a break statement?

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
     typename BeforeStmtAction::result_type beforeStatement(boost::shared_ptr<Stmt> stmt,
							     boost::shared_ptr<Child> child) {
       return(bfrstmt(stmt, child));
      }

     /// Apply the after action.
     template<typename Stmt, typename Child>
     typename AfterStmtAction::result_type afterStatement(boost::shared_ptr<Stmt> stmt,
							   boost::shared_ptr<Child> child) {
       return(aftstmt(stmt, child));
      }

     /// Apply the between action.
     template<typename Stmt, typename Child>
     typename BetweenStmtAction::result_type betweenStatement(boost::shared_ptr<Stmt> stmt,
							      boost::shared_ptr<Child> child1,
							      boost::shared_ptr<Child> child2) {
       return(betstmt(stmt, child1, child2));
      }

     /// Apply the before expression action.
     template<typename Stmt, typename Expr>
     typename BeforeExprAction::result_type beforeExpression(boost::shared_ptr<Stmt> stmt,
							      boost::shared_ptr<Expr> expr) {
       return(bfrexpr(stmt, expr));
      }

     /// Apply the after expression action.
     template<typename Stmt, typename Expr>
     typename AfterExprAction::result_type afterExpression(boost::shared_ptr<Stmt> stmt,
							    boost::shared_ptr<Expr> expr) {
       return(aftexpr(stmt, expr));
      }

     /// Get the expression flow.
     ExprFlow &expressionFlow(void) {
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
     bool hasBreak(void) const {
       return hitBreak;
     }

     /// Clear the hitBreak flag.
     void setNoBreak(void) {
       hitBreak = false;
     }
     /// Set the hitBreak flag.
     void setHasBreak(void) {
       hitBreak = true;
     }
     /// Set the hitBreak flag to a given value.
     void setBreak(bool v) {
       hitBreak = v;
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
