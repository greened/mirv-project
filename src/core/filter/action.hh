#ifndef mirv_core_filter_action_hh
#define mirv_core_filter_action_hh

#include <mirv/core/filter/expression_visitor.hh>
#include <mirv/core/filter/statement_visitor.hh>

namespace mirv {
   class NullAction {
   public:
      typedef void result_type;

      template<typename Node>
      result_type operator()(boost::shared_ptr<Node>) {}
     template<typename Node1, typename Node2>
     result_type operator()(boost::shared_ptr<Node1>,
			    boost::shared_ptr<Node2>) {}
     template<typename Node1, typename Node2, typename Node3>
     result_type operator()(boost::shared_ptr<Node1>,
			    boost::shared_ptr<Node2>,
			    boost::shared_ptr<Node3>) {}
   };

   class VisitStatementAction : public StatementVisitor {
   public:
      template<typename Stmt>
      result_type operator()(boost::shared_ptr<Stmt> stmt) {
	return stmt->accept(*this);
      }
   };
 
  class VisitExpressionAction : public ExpressionVisitor {
   public:
      template<typename Expr>
      result_type operator()(boost::shared_ptr<Expr> expr) {
	return expr->accept(*this);
      }
   };
}

#endif
