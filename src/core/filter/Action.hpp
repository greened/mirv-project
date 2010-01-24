#ifndef mirv_core_filter_Action_hpp
#define mirv_core_filter_Action_hpp

#include <mirv/core/filter/ExpressionVisitor.hpp>
#include <mirv/core/filter/StatementVisitor.hpp>
#include <mirv/core/filter/SymbolVisitor.hpp>

namespace mirv {
  /// This is an action that does nothing.
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

  /// This action takes a statement visitor and sends it to the
  /// statement for execution.
   class VisitStatementAction : public StatementVisitor {
   public:
      template<typename Stmt>
      result_type operator()(boost::shared_ptr<Stmt> stmt) {
	return stmt->accept(*this);
      }
   };
 
  /// This action takes an expression visitor and sends it to the
  /// expression for execution.
  class VisitExpressionAction : public ExpressionVisitor {
   public:
      template<typename Expr>
      result_type operator()(boost::shared_ptr<Expr> expr) {
	return expr->accept(*this);
      }
   };

  /// This action takes a symbol visitor and sends it to the symbol
  /// for execution.
  class VisitSymbolAction : public SymbolVisitor {
   public:
      template<typename Expr>
      result_type operator()(boost::shared_ptr<Expr> sym) {
	return sym->accept(*this);
      }
   };
}

#endif
