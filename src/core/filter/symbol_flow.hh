#ifndef mirv_core_filter_symbol_flow_hh
#define mirv_core_filter_symbol_flow_hh

#include <mirv/core/filter/symbol_visitor.hh>
#include <mirv/core/filter/action.hh>
#include <mirv/core/filter/dataflow.hh>
#include <mirv/core/filter/statement_flow.hh>
#include <mirv/core/ir/module.hh>

namespace mirv {
   template<
     typename EnterAction = NullAction,
     typename LeaveAction = NullAction,
     typename BeforeAction = NullAction,
     typename AfterAction = NullAction,
     typename BetweenAction = NullAction,
     typename StmtFlow = NullStatementFlow>
   class SymbolFlow : public SymbolVisitor {
   private:
      EnterAction ent;
      LeaveAction lve;
      BeforeAction bfr;
      AfterAction aft;
      BetweenAction bet;
      StmtFlow stmt;

   protected:
     template<typename Expr>
     typename EnterAction::result_type enter(boost::shared_ptr<Expr> expr) {
       return(ent(expr));
      };

     template<typename Expr>
     typename LeaveAction::result_type leave(boost::shared_ptr<Expr> expr) {
       return(lve(expr));
      };

     template<typename Expr, typename Child>
     typename BeforeAction::result_type before(boost::shared_ptr<Expr> expr, boost::shared_ptr<Child> child) {
       return(bfr(expr, child));
      };

     template<typename Expr, typename Child>
     typename BeforeAction::result_type between(boost::shared_ptr<Expr> expr, boost::shared_ptr<Child> child1, boost::shared_ptr<Child> child2) {
       return(bfr(expr, child1, child2));
      };

     template<typename Expr, typename Child>
     typename AfterAction::result_type after(boost::shared_ptr<Expr> expr, boost::shared_ptr<Child> child) {
       return(aft(expr, child));
      };

     StmtFlow &statement_flow(void) {
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

     void visit(ptr<Symbol<Module> >::type sym) {
         this->enter(sym);

	 // Visit types
         for(Symbol<Module>::type_iterator t = sym->type_begin(),
                tend = sym->type_end();
             t != tend;
             /* NULL */) {
            this->before(sym, *t);
            (*t)->accept(*this);
            this->after(sym, *t);
            Symbol<Module>::type_iterator prev = t;
            if (++t != tend) {
	      this->between(sym, *prev, *t);
            }
         }

	 // Visit variables
         for(Symbol<Module>::variable_iterator v = sym->variable_begin(),
                vend = sym->variable_end();
             v != vend;
             /* NULL */) {
            this->before(sym, *v);
            (*v)->accept(*this);
            this->after(sym, *v);
            Symbol<Module>::variable_iterator prev = v;
            if (++v != vend) {
	      this->between(sym, *prev, *v);
            }
         }

	 // Visit functions
         for(Symbol<Module>::function_iterator f = sym->function_begin(),
                fend = sym->function_end();
             f != fend;
             /* NULL */) {
            this->before(sym, *f);
            (*f)->accept(*this);
            this->after(sym, *f);
            Symbol<Module>::function_iterator prev = f;
            if (++f != fend) {
	      this->between(sym, *prev, *f);
            }
         }
         this->leave(sym);
      }

     void visit(ptr<Symbol<Function> >::type sym) {
         this->enter(sym);
	 // Visit variables
         for(Symbol<Function>::variable_iterator v = sym->variable_begin(),
                vend = sym->variable_end();
             v != vend;
             /* NULL */) {
            this->before(sym, *v);
            (*v)->accept(*this);
            this->after(sym, *v);
            Symbol<Function>::variable_iterator prev = v;
            if (++v != vend) {
	      this->between(sym, *prev, *v);
            }
         }

	 // Visit statements
	 sym->getStatement()->accept(this->statement_flow());

         this->leave(sym);
      }
   };

  typedef SymbolFlow<> NullSymbolFlow;

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
