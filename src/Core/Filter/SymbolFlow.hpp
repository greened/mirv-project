#ifndef mirv_Core_Filter_SymbolFlow_hpp
#define mirv_Core_Filter_SymbolFlow_hpp

#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/Filter/Dataflow.hpp>
#include <mirv/Core/Filter/StatementFlow.hpp>
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  /// This is a visitor for symbols.  It follows children of inner
  /// symbols such as functions and modules.
   template<
     typename EnterAction = NullAction,
     typename LeaveAction = NullAction,
     typename BeforeAction = NullAction,
     typename AfterAction = NullAction,
     typename BetweenAction = NullAction,
     typename StmtFlow = NullStatementFlow>
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
     /// The flow to send down child statements.
      StmtFlow stmt;

   protected:
     /// Apply the enter action.
     template<typename Expr>
     typename EnterAction::result_type enter(boost::shared_ptr<Expr> expr) {
       return(ent(expr));
      };

     /// Apply the leave action.
     template<typename Expr>
     typename LeaveAction::result_type leave(boost::shared_ptr<Expr> expr) {
       return(lve(expr));
      };

     /// Apply the before action.
     template<typename Expr, typename Child>
     typename BeforeAction::result_type before(boost::shared_ptr<Expr> expr, boost::shared_ptr<Child> child) {
       return(bfr(expr, child));
      };

     /// Apply the between action.
     template<typename Expr, typename Child>
     typename BeforeAction::result_type between(boost::shared_ptr<Expr> expr, boost::shared_ptr<Child> child1, boost::shared_ptr<Child> child2) {
       return(bfr(expr, child1, child2));
      };

     /// Apply the after action.
     template<typename Expr, typename Child>
     typename AfterAction::result_type after(boost::shared_ptr<Expr> expr, boost::shared_ptr<Child> child) {
       return(aft(expr, child));
      };

     /// Get the statement flow.
     StmtFlow &statementFlow(void) {
       return stmt;
     }

   public:
     SymbolFlow(const EnterAction &e = NullAction(),
		const LeaveAction &l = NullAction(),
		const BeforeAction &b = NullAction(),
		const AfterAction &a = NullAction(),
		const BetweenAction &t = NullAction(),
		const StmtFlow &smt = NullStatementFlow())
	: ent(e), lve(l), bfr(b), aft(a), bet(t), stmt(smt) {}

     /// Visit an inner symbol, visiting all children.
     void visit(ptr<InnerSymbol>::type sym) {
         this->enter(sym);
         for(InnerSymbol::iterator s = sym->begin(),
                send = sym->end();
             s != send;
             /* NULL */) {
            this->before(sym, *s);
            (*s)->accept(*this);
            this->after(sym, *s);
            InnerSymbol::iterator prev = s;
            if (++s != send) {
               this->between(sym, *prev, *s);
            }
         }
         this->leave(sym);
      }

     void visit(ptr<LeafSymbol>::type sym) {
         this->enter(sym);
         this->leave(sym);
      }

     void visit(ptr<Symbol<Type<TypeBase> > >::type sym) {
         this->enter(sym);
         this->leave(sym);
      }

     /// Visit a module, visiting contained types, variables and
     /// functions.
     void visit(ptr<Symbol<Module> >::type sym) {
         this->enter(sym);

	 // Visit types
         for(Symbol<Module>::TypeIterator t = sym->typeBegin(),
                tend = sym->typeEnd();
             t != tend;
             /* NULL */) {
            this->before(sym, *t);
            (*t)->accept(*this);
            this->after(sym, *t);
            Symbol<Module>::TypeIterator prev = t;
            if (++t != tend) {
	      this->between(sym, *prev, *t);
            }
         }

	 // Visit variables
         for(Symbol<Module>::VariableIterator v = sym->variableBegin(),
                vend = sym->variableEnd();
             v != vend;
             /* NULL */) {
            this->before(sym, *v);
            (*v)->accept(*this);
            this->after(sym, *v);
            Symbol<Module>::VariableIterator prev = v;
            if (++v != vend) {
	      this->between(sym, *prev, *v);
            }
         }

	 // Visit functions
         for(Symbol<Module>::FunctionIterator f = sym->functionBegin(),
                fend = sym->functionEnd();
             f != fend;
             /* NULL */) {
            this->before(sym, *f);
            (*f)->accept(*this);
            this->after(sym, *f);
            Symbol<Module>::FunctionIterator prev = f;
            if (++f != fend) {
	      this->between(sym, *prev, *f);
            }
         }
         this->leave(sym);
      }

     /// Visit a function, visiting contained variables and
     /// statements.
     void visit(ptr<Symbol<Function> >::type sym) {
         this->enter(sym);
	 // Visit variables
         for(Symbol<Function>::VariableIterator v = sym->variableBegin(),
                vend = sym->variableEnd();
             v != vend;
             /* NULL */) {
            this->before(sym, *v);
            (*v)->accept(*this);
            this->after(sym, *v);
            Symbol<Function>::VariableIterator prev = v;
            if (++v != vend) {
	      this->between(sym, *prev, *v);
            }
         }

	 // Visit statements
	 sym->getStatement()->accept(this->statementFlow());

         this->leave(sym);
      }
   };

  typedef SymbolFlow<> NullSymbolFlow;

  /// This is an object generator for symbol flows.  It uses function
  /// argument type deduction to relieve the user of thhe burden of
  /// specifying all of the template types of a symbol flow.
  template<
    typename EnterAction,
    typename LeaveAction,
    typename BeforeAction,
    typename AfterAction,
    typename BetweenAction,
    typename StmtFlow>
  typename ptr<SymbolFlow<EnterAction,
			  LeaveAction,
			  BeforeAction,
			  AfterAction,
			  BetweenAction,
			  StmtFlow> >::type
  makeSymbolFlow(const EnterAction &e = NullAction(),
		 const LeaveAction &l = NullAction(),
		 const BeforeAction &b = NullAction(),
		 const AfterAction &a = NullAction(),
		 const BetweenAction &t = NullAction(),
		 const StmtFlow &smt = NullStatementFlow()) {
    typename ptr<SymbolFlow<EnterAction,
      LeaveAction,
      BeforeAction,
      AfterAction,
      BetweenAction,
      StmtFlow> >::type flow(new SymbolFlow<EnterAction,
			     LeaveAction,
			     BeforeAction,
			     AfterAction,
			     BetweenAction,
			     StmtFlow>(e, l, b, a, t, smt));
    return flow;
  }
}

#endif