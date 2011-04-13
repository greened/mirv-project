#ifndef mirv_Core_Filter_SymbolFlow_hpp
#define mirv_Core_Filter_SymbolFlow_hpp

#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/Filter/Dataflow.hpp>
#include <mirv/Core/Filter/StatementFlow.hpp>
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
    typename EnterAction::result_type doEnter(boost::shared_ptr<Expr> expr) {
      return(ent(expr));
    };

    /// Apply the leave action.
    template<typename Expr>
    typename LeaveAction::result_type doLeave(boost::shared_ptr<Expr> expr) {
      return(lve(expr));
    };

    /// Apply the before action.
    template<typename Expr, typename InputIterator>
    typename BeforeAction::result_type doBefore(boost::shared_ptr<Expr> expr,
                                                InputIterator child) {
      return(bfr(expr, child));
    };

    /// Apply the between action.
    template<typename Expr, typename InputIterator>
    typename BetweenAction::result_type doBetween(boost::shared_ptr<Expr> expr,
                                                  InputIterator child1,
                                                  InputIterator child2) {
      return(bet(expr, child1, child2));
    };

    /// Apply the after action.
    template<typename Expr, typename InputIterator>
    typename AfterAction::result_type doAfter(boost::shared_ptr<Expr> expr,
                                              InputIterator child) {
      return(aft(expr, child));
    };

    /// Apply the statement action.
    template<typename Expr, typename InputIterator>
    typename StatementAction::result_type doStatement(boost::shared_ptr<Expr> expr,
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

    /// Visit an inner symbol, visiting all children.
    void visit(ptr<InnerSymbol>::type sym) {
      this->doEnter(sym);
      for(InnerSymbol::iterator s = sym->begin(),
            send = sym->end();
          s != send;
          /* NULL */) {
        this->doBefore(sym, s);
        (*s)->accept(*this);
        this->doAfter(sym, s);
        InnerSymbol::iterator prev = s;
        if (++s != send) {
          this->doBetween(sym, prev, s);
        }
      }
      this->doLeave(sym);
    }

    void visit(ptr<LeafSymbol>::type sym) {
      this->doEnter(sym);
      this->doLeave(sym);
    }

    /// Visit a module, visiting contained variables and functions.
    /// We do not visit types as they are immutable.  See
    /// ConstSymbolFlow.
    void visit(ptr<Symbol<Module> >::type sym) {
      this->doEnter(sym);

      // Visit variables
      for(Symbol<Module>::GlobalVariableIterator v = sym->globalVariableBegin(),
            vend = sym->globalVariableEnd();
          v != vend;
          /* NULL */) {
        this->doBefore(sym, v);
        (*v)->accept(*this);
        this->doAfter(sym, v);
        Symbol<Module>::GlobalVariableIterator prev = v;
        if (++v != vend) {
          this->doBetween(sym, prev, v);
        }
      }

      // Visit functions
      for(Symbol<Module>::FunctionIterator f = sym->functionBegin(),
            fend = sym->functionEnd();
          f != fend;
          /* NULL */) {
        this->doBefore(sym, f);
        (*f)->accept(*this);
        this->doAfter(sym, f);
        Symbol<Module>::FunctionIterator prev = f;
        if (++f != fend) {
          this->doBetween(sym, prev, f);
        }
      }
      this->doLeave(sym);
    }

    /// Visit a function, visiting contained variables and
    /// statements.
    void visit(ptr<Symbol<Function> >::type sym) {
      this->doEnter(sym);
      // Visit variables
      for(Symbol<Function>::VariableIterator v = sym->variableBegin(),
            vend = sym->variableEnd();
          v != vend;
          /* NULL */) {
        this->doBefore(sym, v);
        (*v)->accept(*this);
        this->doAfter(sym, v);
        Symbol<Function>::VariableIterator prev = v;
        if (++v != vend) {
          this->doBetween(sym, prev, v);
        }
      }

      // Visit statements
      this->doStatement(sym, sym->statementBegin());

      this->doLeave(sym);
    }
  };

  typedef SymbolFlow<
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullStatementFlow> NullSymbolFlow;

  /// This is a type generator for symbol flows.
  struct SymbolFlowGenerator {
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
  typename ptr<SymbolFlow<EnterAction,
                 LeaveAction,
                 BeforeAction,
                 AfterAction,
                 BetweenAction,
                 StatementAction> >::type
  makeSymbolFlow(const EnterAction &e,
		 const LeaveAction &l,
		 const BeforeAction &b,
		 const AfterAction &a,
		 const BetweenAction &t,
		 const StatementAction &smt) {
    typename ptr<SymbolFlow<EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      BetweenAction,
      StatementAction> >::type flow(new SymbolFlow<EnterAction,
                                    LeaveAction,
                                    BeforeAction,
                                    AfterAction,
                                    BetweenAction,
                                    StatementAction>(e, l, b, a, t, smt));
    return flow;
  }
}

#endif
