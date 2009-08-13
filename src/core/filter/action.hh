#ifndef MIRV_Core_Filter_Action_hh
#define MIRV_Core_Filter_Action_hh

namespace MIRV {
   template<typename R>
   class StatementAction
         : public StatementVisitor<R> {
   public:
      typedef R result_type;

      template<typename Stmt>
      result_type operator(Stmt &stmt) {
         return(visit(stmt));
      }
   };

   template<typename R>
   class ExpressionAction
         : public ExpressionVisitor<R> {
   public:
      typedef R result_type;

      template<typename Expr>
      result_type operator(Expr &expr) {
         return(visit(stmt));
      }
   };
}

#endif
