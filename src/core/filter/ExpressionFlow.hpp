#ifndef mirv_core_filter_ExpressionFlow_hpp
#define mirv_core_filter_ExpressionFlow_hpp

#include <mirv/core/filter/ExpressionVisitor.hpp>
#include <mirv/core/filter/Dataflow.hpp>
#include <mirv/core/ir/Expression.hpp>

namespace mirv {
  /// This is the base class of all flows that operate on expressions.
  template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename BetweenAction = NullAction,
      typename Dataflow = NullDataflow>
   class ExpressionFlow : public ExpressionVisitor {
   private:
    /// The action to apply upon entering an expression.
      EnterAction ent;
    /// The action to apply upon leaving an expression.
      LeaveAction lve;
    /// The action to apply before visiting a child expression.
      BeforeAction bfr;
    /// The action to apply after visiting a child expression.
      AfterAction aft;
    /// The action to apply between visits of child expressions.
      BetweenAction bet;
    /// The dataflow information to propagate.
      Dataflow data;

  protected:
    /// Apply the enter action to an expression.
    template<typename Expr>
     typename EnterAction::result_type enter(boost::shared_ptr<Expr> expr) {
       return(ent(expr));
      };

    /// Apply the leave action to an expression.
     template<typename Expr>
     typename LeaveAction::result_type leave(boost::shared_ptr<Expr> expr) {
       return(lve(expr));
      };

    /// Apply the before action to an expression.
     template<typename Expr, typename Child>
     typename BeforeAction::result_type before(boost::shared_ptr<Expr> expr, boost::shared_ptr<Child> child) {
       return(bfr(expr, child));
      };

    /// Apply the between action to a pair of expressions.
     template<typename Expr, typename Child>
     typename BetweenAction::result_type between(boost::shared_ptr<Expr> expr, boost::shared_ptr<Child> child1, boost::shared_ptr<Child> child2) {
       return(bfr(expr, child1, child2));
      };

    /// Apply the after action to an expression.
     template<typename Expr, typename Child>
     typename AfterAction::result_type after(boost::shared_ptr<Expr> expr, boost::shared_ptr<Child> child) {
       return(aft(expr, child));
      };

    /// Get the current dataflow information.
      Dataflow &dataflow(void) {
         return(data);
      }

   public:
      ExpressionFlow(const EnterAction &e = NullAction(),
                     const LeaveAction &l = NullAction(),
                     const BeforeAction &b = NullAction(),
                     const AfterAction &a = NullAction(),
                     const BetweenAction &t = NullAction(),
                     const Dataflow &d = NullDataflow())
	: ent(e), lve(l), bfr(b), aft(a), bet(t),
	  data(d) {}
   };

  /// An expression flow that does nothing.
  typedef ExpressionFlow<> NullExpressionFlow;

  /// This is an expression flow that walks expressions leftmost
  /// operands first.
   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename BetweenAction = NullAction,
      typename Dataflow = NullDataflow>
   class ForwardExpressionFlow
         : public ExpressionFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      BetweenAction,
      Dataflow> {
     typedef ExpressionFlow<
       EnterAction,
       LeaveAction,
       BeforeAction,
       AfterAction,
       BetweenAction,
       Dataflow> BaseType;

   public:
      ForwardExpressionFlow(const EnterAction &e,
                            const LeaveAction &l,
                            const BeforeAction &b,
                            const AfterAction &a,
                            const BetweenAction &t,
                            const Dataflow &d)
	: BaseType(e, l, b, a, t, d) {}

     /// Visit inner expressions from leftmost operand to rightmost
     /// operand.
      void visit(ptr<InnerExpression>::type expr) {
         this->enter(expr);

         for(InnerExpression::iterator i = expr->begin(),
                iend = expr->end();
             i != iend;
             /* NULL */) {
            this->before(expr, *i);
            (*i)->accept(*this);
            this->after(expr, *i);
            InnerExpression::iterator prev = i;
            if (++i != iend) {
               this->between(expr, *prev, *i);
            }
         }

         this->leave(expr);
      }
     /// Visit all other expressions.
      void visit(ptr<LeafExpression>::type expr) {
         this->enter(expr);
         this->leave(expr);
      }
   };

  /// This is an expression flow that walks expressions from rightmost
  /// operand to leftmost operand.
   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename BetweenAction = NullAction,
      typename Dataflow = NullDataflow>
   class BackwardExpressionFlow
         : public ExpressionFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      BetweenAction,
      Dataflow> {
     typedef ExpressionFlow<
       EnterAction,
       LeaveAction,
       BeforeAction,
       AfterAction,
       BetweenAction,
       Dataflow> BaseType;

   public:
      BackwardExpressionFlow(const EnterAction &e,
                             const LeaveAction &l,
                             const BeforeAction &b,
                             const AfterAction &a,
                             const BetweenAction &t,
                             const Dataflow &d)
	: BaseType(e, l, b, a, t, d) {}

     /// Walk inner expressions from rightmost operand to leftmost
     /// operand.
      void visit(ptr<InnerExpression>::type expr) {
         this->enter(expr);

         for(InnerExpression::reverse_iterator i = expr->rbegin(),
                iend = expr->rend();
             i != iend;
             /* NULL */) {
            this->before(expr, *i);
            (*i)->accept(*this);
            this->after(expr, *i);
            InnerExpression::reverse_iterator prev = i;
            if (++i != iend) {
               this->between(expr, *prev, *i);
            }
         } 

         this->leave(expr);
      }

     /// Walk all other expressions.
      void visit(ptr<LeafExpression>::type expr) {
         this->enter(expr);
         this->leave(expr);
      }
   };
}

#endif
