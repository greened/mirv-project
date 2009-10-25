#ifndef mirv_core_filter_expression_flow_hh
#define mirv_core_filter_expression_flow_hh

#include <mirv/filter/visitor.hh>
#include <mirv/filter/dataflow.hh>

namespace mirv {
   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename Dataflow = NullDataflow>
   class ExpressionFlow : public ExpressionVisitor {
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
      ExpressionFlow(const EnterAction &e,
                     const LeaveAction &l,
                     const BeforeAction &b,
                     const AfterAction &a,
                     const Dataflow &d)
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
      ForwardExpressionFlow(const EnterAction &e,
                            const LeaveAction &l,
                            const BeforeAction &b,
                            const AfterAction &a,
                            const Dataflow &d)
	: BaseType(e, l, b, a, d) {}

      void visit(InnerExpression &expr) {
         this->enter(expr);

         for(InnerExpression::iterator i = expr.begin(),
                iend = expr.end();
             i != iend;
             /* NULL */) {
            this->before(expr, **i);
            (*i)->accept(*this);
            this->after(expr, **i);
            InnerExpression::iterator prev = i;
            if (++i != iend) {
               this->between(expr, **prev, **i);
            }
         }

         this->leave(expr);
      }
      void visit(LeafExpression &expr) {
         this->enter(expr);
         this->leave(expr);
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
      BackwardExpressionFlow(const EnterAction &e,
                             const LeaveAction &l,
                             const BeforeAction &b,
                             const AfterAction &a,
                             const Dataflow &d)
            : BaseType(e, l, b, a, d) {}

      void visit(InnerExpression &expr) {
         this->enter(expr);

         for(InnerExpression::reverse_iterator i = expr.rbegin(),
                iend = expr.rend();
             i != iend;
             /* NULL */) {
            this->before(expr, **i);
            (*i)->accept(*this);
            this->after(expr, **i);
            InnerExpression::reverse_iterator prev = i;
            if (++i != iend) {
               this->between(expr, **prev, **i);
            }
         } 

         this->leave(expr);
      }

      void visit(LeafExpression &expr) {
         this->enter(expr);
         this->leave(expr);
      }
   };
}

#endif
