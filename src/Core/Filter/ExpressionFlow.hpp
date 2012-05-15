#ifndef mirv_Core_Filter_ExpressionFlow_hpp
#define mirv_Core_Filter_ExpressionFlow_hpp

#include <mirv/Core/Filter/ExpressionVisitor.hpp>
#include <mirv/Core/Filter/Dataflow.hpp>
#include <mirv/Core/IR/Expression.hpp>

namespace mirv {
  /// This is the base class of all flows that operate on expressions.
  template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeAction,
      typename AfterAction,
      typename BetweenAction
    >
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

  protected:
    EnterAction &enterExpression(void) {
      return ent;
    }

    LeaveAction &leaveExpression(void) {
      return lve;
    }

    BeforeAction &beforeExpression(void) {
      return bfr;
    }

    BetweenAction &betweenExpression(void) {
      return bet;
    }

    AfterAction &afterExpression(void) {
      return aft;
    }

    /// Apply the enter action to an expression.
    template<typename Expr>
     typename EnterAction::result_type doEnter(boost::shared_ptr<Expr> expr) {
       return(ent(expr));
      };

    /// Apply the leave action to an expression.
     template<typename Expr>
     typename LeaveAction::result_type doLeave(boost::shared_ptr<Expr> expr) {
       return(lve(expr));
      };

    /// Apply the before action to an expression.
     template<typename Expr, typename InputIterator>
     typename BeforeAction::result_type doBefore(boost::shared_ptr<Expr> expr,
                                                 InputIterator child) {
       return(bfr(expr, child));
      };

    /// Apply the between action to a pair of expressions.
     template<typename Expr, typename InputIterator>
     typename BetweenAction::result_type doBetween(boost::shared_ptr<Expr> expr,
                                                   InputIterator child1,
                                                   InputIterator child2) {
       return(bfr(expr, child1, child2));
      };

    /// Apply the after action to an expression.
     template<typename Expr, typename InputIterator>
     typename AfterAction::result_type doAfter(boost::shared_ptr<Expr> expr,
                                               InputIterator child) {
       return(aft(expr, child));
      };

   public:
      ExpressionFlow(const EnterAction &e,
                     const LeaveAction &l,
                     const BeforeAction &b,
                     const AfterAction &a,
                     const BetweenAction &t)
	: ent(e), lve(l), bfr(b), aft(a), bet(t) {}

    // Allow in-place construction of actions.
    template<typename ...Args>
    ExpressionFlow(Args &...args)
	: ent(args...), lve(args...), bfr(args...), aft(args...),
            bet(args...) {}

    template<typename Flow>
    void transfer(Flow &other) {}
   };

  /// An expression flow that does nothing.
  typedef ExpressionFlow<
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction
    > NullExpressionFlow;

  /// This is an expression flow that walks expressions leftmost
  /// operands first.
   template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeAction,
      typename AfterAction,
      typename BetweenAction
     >
   class ForwardExpressionFlow
         : public ExpressionFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      BetweenAction
     > {
     typedef ExpressionFlow<
       EnterAction,
       LeaveAction,
       BeforeAction,
       AfterAction,
       BetweenAction
       > BaseType;

   public:
      ForwardExpressionFlow(const EnterAction &e,
                            const LeaveAction &l,
                            const BeforeAction &b,
                            const AfterAction &a,
                            const BetweenAction &t)
	: BaseType(e, l, b, a, t) {}

     // Allow in-place construction of actions.
     template<typename ...Args>
     ForwardExpressionFlow(Args &...args) : BaseType(args...) {}

     /// Visit inner expressions from leftmost operand to rightmost
     /// operand.
      void visit(ptr<InnerExpression> expr) {
         this->doEnter(expr);

         for(InnerExpression::iterator i = expr->begin(),
                iend = expr->end();
             i != iend;
             /* NULL */) {
            this->doBefore(expr, i);
            (*i)->accept(*this);
            this->doAfter(expr, i);
            InnerExpression::iterator prev = i;
            if (++i != iend) {
               this->doBetween(expr, prev, i);
            }
         }

         this->doLeave(expr);
      }
     /// Visit all other expressions.
      void visit(ptr<LeafExpression> expr) {
         this->doEnter(expr);
         this->doLeave(expr);
      }
   };

     /// This is a type generator for ForwardExpressionFlow, needed by
     /// AttributeFlow.
     struct ForwardExpressionFlowGenerator {
       template<
         typename EnterAction,
         typename LeaveAction,
         typename BeforeAction,
         typename AfterAction,
         typename BetweenAction
         >
       /// This is a metafunction to generate a ForwardExpressionFlow
       /// with the given configuration.
       struct apply {
         typedef ForwardExpressionFlow<
         EnterAction,
         LeaveAction,
         BeforeAction,
	 AfterAction,
           BetweenAction
           > type;
       };
     };

  /// This is an expression flow that walks expressions from rightmost
  /// operand to leftmost operand.
   template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeAction,
      typename AfterAction,
      typename BetweenAction
     >
   class BackwardExpressionFlow
         : public ExpressionFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      BetweenAction
      > {
     typedef ExpressionFlow<
       EnterAction,
       LeaveAction,
       BeforeAction,
       AfterAction,
       BetweenAction
       > BaseType;

   public:
      BackwardExpressionFlow(const EnterAction &e,
                             const LeaveAction &l,
                             const BeforeAction &b,
                             const AfterAction &a,
                             const BetweenAction &t)
	: BaseType(e, l, b, a, t) {}

     // Allow in-place construction of actions.
     template<typename ...Args>
     BackwardExpressionFlow(Args &...args) : BaseType(args...) {}

     /// Walk inner expressions from rightmost operand to leftmost
     /// operand.
      void visit(ptr<InnerExpression> expr) {
         this->doEnter(expr);

         for(InnerExpression::reverse_iterator i = expr->rbegin(),
                iend = expr->rend();
             i != iend;
             /* NULL */) {
            this->doBefore(expr, i);
            (*i)->accept(*this);
            this->doAfter(expr, i);
            InnerExpression::reverse_iterator prev = i;
            if (++i != iend) {
               this->doBetween(expr, prev, i);
            }
         } 

         this->doLeave(expr);
      }

     /// Walk all other expressions.
      void visit(ptr<LeafExpression> expr) {
         this->doEnter(expr);
         this->doLeave(expr);
      }
   };

     /// This is a type generator for BackwardExpressionFlow, needed by
     /// AttributeFlow.
     struct BackwardExpressionFlowGenerator {
       template<
         typename EnterAction,
         typename LeaveAction,
         typename BeforeAction,
         typename AfterAction,
         typename BetweenAction
         >
       /// This is the metafunction to return a BackwardExpressionFlow
       /// type with the given configuration.
       struct apply {
         typedef BackwardExpressionFlow<
         EnterAction,
         LeaveAction,
         BeforeAction,
	 AfterAction,
           BetweenAction
           > type;
       };
     };
}

#endif
