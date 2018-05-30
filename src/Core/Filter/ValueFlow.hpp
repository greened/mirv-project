#ifndef mirv_Core_Filter_ValueFlow_hpp
#define mirv_Core_Filter_ValueFlow_hpp

#include <mirv/Core/Filter/ValueVisitor.hpp>
#include <mirv/Core/Filter/Dataflow.hpp>
#include <mirv/Core/IR/ValueProducer.hpp>

namespace mirv {
  /// This is the base class of all flows that operate on expressions.
  template<
    typename EnterAction,
    typename LeaveAction,
    typename BeforeAction,
    typename AfterAction,
    typename BetweenAction
    >
  class ValueFlow : public ValueVisitor {
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
    EnterAction &enterProducer(void) {
      return ent;
    }

    LeaveAction &leaveProducer(void) {
      return lve;
    }

    BeforeAction &beforeProducer(void) {
      return bfr;
    }

    BetweenAction &betweenProducer(void) {
      return bet;
    }

    AfterAction &afterProducer(void) {
      return aft;
    }

    /// Apply the enter action to an expression.
    template<typename Expr>
    typename EnterAction::result_type doEnter(Expr & expr) {
      return(ent(expr));
    };

    /// Apply the leave action to an expression.
    template<typename Expr>
    typename LeaveAction::result_type doLeave(Expr & expr) {
      return(lve(expr));
    };

    /// Apply the before action to an expression.
    template<typename Expr, typename Child>
    typename BeforeAction::result_type doBefore(Expr & expr,
                                                ptr<Child> child) {
      return(bfr(expr, child));
    };

    /// Apply the between action to a pair of expressions.
    template<typename Expr, typename Child1, typename Child2>
    typename BetweenAction::result_type doBetween(Expr & expr,
                                                  ptr<Child1> child1,
                                                  ptr<Child2> child2) {
      return(bet(expr, child1, child2));
    };

    /// Apply the after action to an expression.
    template<typename Expr, typename Child>
    typename AfterAction::result_type doAfter(Expr & expr,
                                              ptr<Child> child) {
      return(aft(expr, child));
    };

  public:
    ValueFlow(const EnterAction &e,
              const LeaveAction &l,
              const BeforeAction &b,
              const AfterAction &a,
              const BetweenAction &t)
      : ent(e), lve(l), bfr(b), aft(a), bet(t) {}

    // Allow in-place construction of actions.
    template<typename ...Args>
    ValueFlow(Args &...args)
      : ent(args...), lve(args...), bfr(args...), aft(args...),
        bet(args...) {}

    template<typename Flow>
    void transfer(Flow &other) {}
  };

  /// An expression flow that does nothing.
  typedef ValueFlow<
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction
    > NullValueFlow;

  /// This is an expression flow that walks expressions leftmost
  /// operands first.
  template<
    typename EnterAction,
    typename LeaveAction,
    typename BeforeAction,
    typename AfterAction,
    typename BetweenAction
    >
  class ForwardValueFlow
    : public ValueFlow<
    EnterAction,
    LeaveAction,
    BeforeAction,
    AfterAction,
    BetweenAction
    > {
    typedef ValueFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      BetweenAction
      > BaseType;

  public:
    ForwardValueFlow(const EnterAction &e,
                     const LeaveAction &l,
                     const BeforeAction &b,
                     const AfterAction &a,
                     const BetweenAction &t)
      : BaseType(e, l, b, a, t) {}

    // Allow in-place construction of actions.
    template<typename ...Args>
    ForwardValueFlow(Args &...args) : BaseType(args...) {}

    /// Visit inner expressions from leftmost operand to rightmost
    /// operand.
    void visit(ValueConsumer & expr) override {
      this->doEnter(expr);

      ptr<ValueProducer> Prev;
      for(auto P : expr.Operands()) {
        if (Prev) {
          this->doBetween(expr, Prev, P);
        }
        this->doBefore(expr, P);
        P->accept(*this);
        this->doAfter(expr, P);
      }

      this->doLeave(expr);
    }

    /// Visit all other expressions.
    void visit(ValueProducer & expr) override {
      this->doEnter(expr);
      this->doLeave(expr);
    }
  };

  /// This is a type generator for ForwardValueFlow, needed by
  /// AttributeFlow.
  struct ForwardValueFlowGenerator {
    template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeAction,
      typename AfterAction,
      typename BetweenAction
      >
    /// This is a metafunction to generate a ForwardValueFlow
    /// with the given configuration.
    struct apply {
      typedef ForwardValueFlow<
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
  class BackwardValueFlow
    : public ValueFlow<
    EnterAction,
    LeaveAction,
    BeforeAction,
    AfterAction,
    BetweenAction
    > {
    typedef ValueFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      BetweenAction
      > BaseType;

  public:
    BackwardValueFlow(const EnterAction &e,
                      const LeaveAction &l,
                      const BeforeAction &b,
                      const AfterAction &a,
                      const BetweenAction &t)
      : BaseType(e, l, b, a, t) {}

    // Allow in-place construction of actions.
    template<typename ...Args>
    BackwardValueFlow(Args &...args) : BaseType(args...) {}

    /// Walk inner expressions from rightmost operand to leftmost
    /// operand.
    void visit(ValueConsumer & expr) override {
      this->doEnter(expr);

      ptr<ValueProducer> Prev;
      for(auto P : expr.Operands()) {
        if (Prev) {
          this->doBetween(expr, Prev, P);
        }
        this->doBefore(expr, P);
        P->accept(*this);
        this->doAfter(expr, P);
      }

      this->doLeave(expr);
    }

    /// Walk all other expressions.
    void visit(ValueProducer & expr) override {
      this->doEnter(expr);
      this->doLeave(expr);
    }
  };

  /// This is a type generator for BackwardValueFlow, needed by
  /// AttributeFlow.
  struct BackwardValueFlowGenerator {
    template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeAction,
      typename AfterAction,
      typename BetweenAction
      >
    /// This is the metafunction to return a BackwardValueFlow
    /// type with the given configuration.
    struct apply {
      typedef BackwardValueFlow<
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
