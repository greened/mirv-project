#ifndef mirv_Core_Filter_SymbolFlow_hpp
#define mirv_Core_Filter_SymbolFlow_hpp

#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/Filter/Dataflow.hpp>
#include <mirv/Core/Filter/ControlFlow.hpp>
#include <mirv/Core/IR/Module.hpp>

#include <vector>

namespace mirv {
  /// This is a visitor for symbols.  It follows children of inner
  /// symbols such as functions and modules.
  template<
    typename EnterAction,
    typename LeaveAction,
    typename BeforeAction,
    typename AfterAction,
    typename BetweenAction,
    typename StatementAction>
  class SymbolFlow : public SymbolVisitor {
  private:
    /// The acion to invoke upon entry to a symbol.
    EnterAction ent;
    /// The acion to invoke upon leaving a symbol.
    LeaveAction lve;
    /// The action to invoke before visiting a child symbol.
    BeforeAction bfr;
    /// The action to invoke after visiting a child symbol.
    AfterAction aft;
    /// The action to invoke between visits of child symbols.
    BetweenAction bet;
    /// The flow to invoke on child statements.
    StatementAction stmt;

  protected:
    EnterAction &enter(void) {
      return ent;
    }

    LeaveAction &leave(void) {
      return lve;
    }

    BeforeAction &before(void) {
      return bfr;
    }

    BetweenAction &between(void) {
      return bet;
    }

    AfterAction &after(void) {
      return aft;
    }

    StatementAction &statement(void) {
      return stmt;
    }

    /// Apply the enter action.
    template<typename Expr>
    typename EnterAction::result_type doEnter(Expr & expr) {
      return(ent(expr));
    };

    /// Apply the leave action.
    template<typename Expr>
    typename LeaveAction::result_type doLeave(Expr & expr) {
      return(lve(expr));
    };

    /// Apply the before action.
    template<typename Expr, typename InputIterator>
    typename BeforeAction::result_type doBefore(Expr & expr,
                                                InputIterator child) {
      return(bfr(expr, child));
    };

    /// Apply the between action.
    template<typename Expr, typename InputIterator>
    typename BetweenAction::result_type doBetween(Expr & expr,
                                                  InputIterator child1,
                                                  InputIterator child2) {
      return(bet(expr, child1, child2));
    };

    /// Apply the after action.
    template<typename Expr, typename InputIterator>
    typename AfterAction::result_type doAfter(Expr & expr,
                                              InputIterator child) {
      return(aft(expr, child));
    };

    /// Apply the statement action.
    template<typename Expr, typename InputIterator>
    typename StatementAction::result_type doStatement(Expr & expr,
                                                      InputIterator child) {
      return stmt(expr, child);
    };

  public:
    SymbolFlow(const EnterAction &e,
               const LeaveAction &l,
               const BeforeAction &b,
               const AfterAction &a,
               const BetweenAction &t,
               const StatementAction &smt)
	: ent(e), lve(l), bfr(b), aft(a), bet(t), stmt(smt) {}

    // Allow in-place construction of actions.
    template <typename ...Args>
    SymbolFlow(Args &...args)
        : ent(args...), lve(args...), bfr(args...), aft(args...),
            bet(args...), stmt(args...) {}

    template<typename Flow>
    void transfer(Flow &other) {}

    /// Visit a module, visiting contained types.
    void visit(Module & sym) override {
      this->doEnter(sym);

      ptr<const Type> PrevT = nullptr;
      for (auto T : sym.Types()) {
        if (PrevT) {
          this->doBetween(sym, PrevT, T);
        }

        this->doBefore(sym, T);
        T->accept(*this);
        this->doAfter(sym, T);

        PrevT = T;
      }

      ptr<GlobalVariable> PrevGV = nullptr;
      for (auto GV : sym.Globals()) {
        if (PrevGV) {
          this->doBetween(sym, PrevGV, GV);
        }

        this->doBefore(sym, GV);
        GV->accept(*this);
        this->doAfter(sym, GV);

        PrevGV = GV;
      }

      // ptr<Constant> PrevC = nullptr;
      // for (auto C : sym.Constants()) {
      //   if (PrevC) {
      //     this->doBetween(sym, PrevC, C);
      //   }

      //   this->doBefore(sym, C);
      //   C->accept(*this);
      //   this->doAfter(sym, C);

      //   PrevC = C;
      // }

      ptr<Function> PrevF = nullptr;
      for (auto F : sym.Functions()) {
        if (PrevF) {
          this->doBetween(sym, PrevF, F);
        }

        this->doBefore(sym, F);
        F->accept(*this);
        this->doAfter(sym, F);

        PrevF = F;
      }

      this->doLeave(sym);
    }

    /// Visit a function
    void visit(Function & sym) override {
      this->doEnter(sym);
      this->doLeave(sym);
    }
  };

  typedef SymbolFlow<
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullControlFlow> NullSymbolFlow;

  /// This is a type generator for symbol flows.
  struct SymbolFlowGenerator {
    /// This is a metafunction to generate a SymbolFlow with the
    /// provided configuration.
    template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeAction,
      typename AfterAction,
      typename BetweenAction,
      typename StatementAction>
    struct apply {
      typedef SymbolFlow<EnterAction, LeaveAction, BeforeAction,
        AfterAction, BetweenAction, StatementAction> type;
    };
  };

  /// This is an object generator for symbol flows.  It uses function
  /// argument type deduction to relieve the user of thhe burden of
  /// specifying all of the template types of a symbol flow.
  template<
    typename EnterAction,
    typename LeaveAction,
    typename BeforeAction,
    typename AfterAction,
    typename BetweenAction,
    typename StatementAction>
  ptr<SymbolFlow<EnterAction,
                 LeaveAction,
                 BeforeAction,
                 AfterAction,
                 BetweenAction,
                 StatementAction> >
  makeSymbolFlow(const EnterAction &e,
		 const LeaveAction &l,
		 const BeforeAction &b,
		 const AfterAction &a,
		 const BetweenAction &t,
		 const StatementAction &smt) {
    ptr<SymbolFlow<EnterAction,
                   LeaveAction,
                   BeforeAction,
                   AfterAction,
                   BetweenAction,
                   StatementAction> >
      flow(new SymbolFlow<EnterAction,
           LeaveAction,
           BeforeAction,
           AfterAction,
           BetweenAction,
           StatementAction>(e, l, b, a, t, smt));
    return flow;
  }
}

#endif
