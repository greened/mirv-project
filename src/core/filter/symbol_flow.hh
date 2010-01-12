#ifndef mirv_core_filter_symbol_flow_hh
#define mirv_core_filter_symbol_flow_hh

#include <mirv/core/filter/visitor.hh>
#include <mirv/core/filter/dataflow.hh>
#include <mirv/core/ir/module.hh>

namespace mirv {
   template<
      typename EnterAction = NullAction,
      typename LeaveAction = NullAction,
      typename BeforeAction = NullAction,
      typename AfterAction = NullAction,
      typename BetweenAction = NullAction>
   class SymbolFlow : public SymbolVisitor {
   private:
      EnterAction ent;
      LeaveAction lve;
      BeforeAction bfr;
      AfterAction aft;
      BetweenAction bet;

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

   public:
      SymbolFlow(const EnterAction &e,
		 const LeaveAction &l,
		 const BeforeAction &b,
		 const AfterAction &a,
		 const BetweenAction &t)
	: ent(e), lve(l), bfr(b), aft(a), bet(t) {}

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
   };

  typedef SymbolFlow<> NullSymbolFlow;
}

#endif
