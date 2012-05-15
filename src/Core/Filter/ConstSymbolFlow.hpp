#ifndef mirv_Core_Filter_ConstSymbolFlow_hpp
#define mirv_Core_Filter_ConstSymbolFlow_hpp

#include <mirv/Core/Filter/ConstSymbolVisitor.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/Filter/Dataflow.hpp>
#include <mirv/Core/Filter/StatementFlow.hpp>
#include <mirv/Core/IR/Module.hpp>

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
  class ConstSymbolFlow : public ConstSymbolVisitor {
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

    typedef std::vector<ptr<const Symbol<Type<TypeBase> > >> TypeList;
    TypeList TypeStack;
 
  protected:
    typedef TypeList::reverse_iterator TypeStackIterator;
    typedef TypeList::const_reverse_iterator ConstTypeStackIterator;

    TypeStackIterator typeStackBegin(void) {
      return TypeStack.rbegin();
    }
    ConstTypeStackIterator typeStackBegin(void) const{
      return TypeStack.rbegin();
    }

    TypeStackIterator typeStackEnd(void) {
      return TypeStack.rend();
    }
    ConstTypeStackIterator typeStackEnd(void) const{
      return TypeStack.rend();
    }

    TypeStackIterator
    typeStackFind(ptr<const Symbol<Type<TypeBase> > > type) {
      return std::find(typeStackBegin(), typeStackEnd(), type);
    }
    ConstTypeStackIterator
    typeStackFind(ptr<const Symbol<Type<TypeBase> > > type) const {
      return std::find(typeStackBegin(), typeStackEnd(), type);
    }

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
    ConstSymbolFlow(const EnterAction &e,
                    const LeaveAction &l,
                    const BeforeAction &b,
                    const AfterAction &a,
                    const BetweenAction &t,
                    const StatementAction &smt)
	: ent(e), lve(l), bfr(b), aft(a), bet(t), stmt(smt) {}

    // Allow in-place construction of actions.
    template <typename ...Args>
    ConstSymbolFlow(Args &...args)
        : ent(args...), lve(args...), bfr(args...), aft(args...),
            bet(args...), stmt(args...) {}

    template<typename Flow>
    void transfer(Flow &other) {}

    /// Visit an inner symbol, visiting all children.
    void visit(ptr<const InnerSymbol> sym) {
      this->doEnter(sym);
      for(InnerSymbol::const_iterator s = sym->begin(),
            send = sym->end();
          s != send;
          /* NULL */) {
        this->doBefore(sym, s);
        (*s)->accept(*this);
        this->doAfter(sym, s);
        InnerSymbol::const_iterator prev = s;
        if (++s != send) {
          this->doBetween(sym, prev, s);
        }
      }
      this->doLeave(sym);
    }

    /// Visit an inner type, visiting all children.  Track visited
    /// types to avoid infinite recursion.
    void visit(ptr<const InnerType> sym) {
      TypeStack.push_back(sym);
      this->doEnter(sym);
      for(InnerType::const_iterator s = sym->begin(),
            send = sym->end();
          s != send;
          /* NULL */) {
        this->doBefore(sym, s);
        if (*s && typeStackFind(*s) == typeStackEnd()) {
          (*s)->accept(*this);
        }
        this->doAfter(sym, s);
        InnerType::const_iterator prev = s;
        if (++s != send) {
          this->doBetween(sym, prev, s);
        }
      }
      this->doLeave(sym);
      TypeStack.pop_back();
    }

    void visit(ptr<const LeafSymbol> sym) {
      this->doEnter(sym);
      this->doLeave(sym);
    }

    void visit(ptr<const Symbol<Type<TypeBase> > > sym) {
      this->doEnter(sym);
      this->doLeave(sym);
    }

    /// Visit a module, visiting contained types, variables and
    /// functions.
    void visit(ptr<const Symbol<Module> > sym) {
      this->doEnter(sym);

      // Visit types
      for(Symbol<Module>::ConstTypeIterator t = sym->typeBegin(),
            tend = sym->typeEnd();
          t != tend;
          /* NULL */) {
        this->doBefore(sym, t);
        (*t)->accept(*this);
        this->doAfter(sym, t);
        Symbol<Module>::ConstTypeIterator prev = t;
        if (++t != tend) {
          this->doBetween(sym, prev, t);
        }
      }

      // Visit variables
      for(Symbol<Module>::ConstGlobalVariableIterator v =
            sym->globalVariableBegin(),
            vend = sym->globalVariableEnd();
          v != vend;
          /* NULL */) {
        this->doBefore(sym, v);
        (*v)->accept(*this);
        this->doAfter(sym, v);
        Symbol<Module>::ConstGlobalVariableIterator prev = v;
        if (++v != vend) {
          this->doBetween(sym, prev, v);
        }
      }

      // Visit functions
      for(Symbol<Module>::ConstFunctionIterator f = sym->functionBegin(),
            fend = sym->functionEnd();
          f != fend;
          /* NULL */) {
        this->doBefore(sym, f);
        (*f)->accept(*this);
        this->doAfter(sym, f);
        Symbol<Module>::ConstFunctionIterator prev = f;
        if (++f != fend) {
          this->doBetween(sym, prev, f);
        }
      }
      this->doLeave(sym);
    }

    /// Visit a function, visiting contained variables and
    /// statements.
    void visit(ptr<const Symbol<Function> > sym) {
      this->doEnter(sym);
      // Visit variables
      for(Symbol<Function>::ConstVariableIterator v = sym->variableBegin(),
            vend = sym->variableEnd();
          v != vend;
          /* NULL */) {
        this->doBefore(sym, v);
        (*v)->accept(*this);
        this->doAfter(sym, v);
        Symbol<Function>::ConstVariableIterator prev = v;
        if (++v != vend) {
          this->doBetween(sym, prev, v);
        }
      }

      // Visit statements
      this->doStatement(sym, sym->statementBegin());

      this->doLeave(sym);
    }
  };

  typedef ConstSymbolFlow<
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullAction,
    NullStatementFlow> NullConstSymbolFlow;

  /// This is a type generator for symbol flows.
  struct ConstSymbolFlowGenerator {
    /// This is a metafunction to generate a ConstSymbolFlow type with
    /// the given configuration.
    template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeAction,
      typename AfterAction,
      typename BetweenAction,
      typename StatementAction>
    struct apply {
      typedef ConstSymbolFlow<EnterAction, LeaveAction, BeforeAction,
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
  typename ptr<ConstSymbolFlow<
                 EnterAction,
                 LeaveAction,
                 BeforeAction,
                 AfterAction,
                 BetweenAction,
                 StatementAction> >::type
  makeConstSymbolFlow(const EnterAction &e,
                      const LeaveAction &l,
                      const BeforeAction &b,
                      const AfterAction &a,
                      const BetweenAction &t,
                      const StatementAction &smt) {
    typename ptr<ConstSymbolFlow<
      EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      BetweenAction,
      StatementAction> >::type flow(new ConstSymbolFlow<
                                      EnterAction,
                                      LeaveAction,
                                      BeforeAction,
                                      AfterAction,
                                      BetweenAction,
                                      StatementAction>(e, l, b, a, t, smt));
    return flow;
  }
}

#endif
