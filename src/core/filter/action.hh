#ifndef mirv_core_filter_action_hh
#define mirv_core_filter_action_hh

#include <mirv/filter/visitor.hh>

namespace mirv {
   class NullAction {
   public:
      typedef void result_type;

      template<typename Node>
      result_type operator()(Node &) {}
   };

   class VisitStatementAction : public StatementVisitor {
   public:
      template<typename Stmt>
      result_type operator()(Stmt &stmt) {
         return(visit(stmt));
      }
   };
 
  class VisitExpressionAction : public ExpressionVisitor {
   public:
      template<typename Expr>
      result_type operator()(Expr &expr) {
         return(visit(expr));
      }
   };
}

#endif
