#ifndef MIRV_Filter_ExpressionFlow_hh
#define MIRV_Filter_ExpressionFlow_hh

#include <mirv/filter/visitor.hh>
#include <mirv/filter/dataflow.hh>

namespace MIRV {
   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename Dataflow = NullDataflow>
   class ExpressionFlow
     : public ExpressionVisitor<void>::type {
   private:
      EnterAction ent;
      LeaveAction lve;
      BeforeAction bfr;
      AfterAction aft;
      Dataflow data;

   protected:
     template<typename Expr>
     typename EnterAction::result_type enter(Expr &expr) {
       return(ent(expr));
      };

     template<typename Expr>
     typename LeaveAction::result_type leave(Expr &expr) {
       return(lve(expr));
      };

     template<typename Expr>
     typename BeforeAction::result_type before(Expr &expr) {
       return(bfr(expr));
      };

     template<typename Expr>
     typename AfterAction::result_type after(Expr &expr) {
       return(aft(expr));
      };

      Dataflow &dataflow(void) {
         return(data);
      }

   public:
      ExpressionFlow(EnterAction &e,
                     LeaveAction &l,
                     BeforeAction &b,
                     AfterAction &a,
                     Dataflow &d)
            : ent(e), lve(l), bfr(b), aft(a),
                 data(d) {}
   };

  typedef ExpressionFlow<> NullExpressionFlow;

   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename Dataflow = NullDataflow>
   class ForwardExpressionFlow
         : public ExpressionFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      Dataflow> {
     typedef ExpressionFlow<
       EnterAction,
       LeaveAction,
       BeforeAction,
       AfterAction,
       Dataflow> BaseType;

   public:
      ForwardExpressionFlow(EnterAction &e,
                            LeaveAction &l,
                            BeforeAction &b,
                            AfterAction &a,
                            Dataflow &d)
	: BaseType(e, l, b, a, d) {}

      void visit(BaseExpression &expr) {
         enter(expr);

         for(BaseExpression::iterator i = expr.begin(),
                iend = expr.end();
             i != iend;
             ;) {
            before(expr, **i);
            (*i)->accept(*this);
            after(expr, **i);
            BaseExpression::iterator prev = i;
            if (++i != iend) {
               between(expr, **prev, **i);
            }
         }

         leave(expr);
      }
   };

   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename Dataflow = NullDataflow>
   class BackwardExpressionFlow
         : public ExpressionFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      Dataflow> {
     typedef ExpressionFlow<
       EnterAction,
       LeaveAction,
       BeforeAction,
       AfterAction,
       Dataflow> BaseType;

   public:
      BackwardExpressionFlow(EnterAction &e,
                             LeaveAction &l,
                             BeforeAction &b,
                             AfterAction &a,
                             Dataflow &d)
            : BaseType(e, l, b, a, d) {}

      void visit(BaseExpression &expr) {
         enter(expr);

         for(BaseExpression:reverse_:iterator i = expr.rbegin(),
                iend = expr.rend();
             i != iend;
             ;) {
            before(expr, **i);
            (*i)->accept(*this);
            after(expr, **i);
            BaseExpression::iterator prev = i;
            if (++i != iend) {
               between(expr, **prev, **i);
            }
         }

         leave(expr);
      }
   };
}

#endif
