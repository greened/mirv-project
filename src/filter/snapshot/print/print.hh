#ifndef mirv_filter_snapshot_print_hh
#define mirv_filter_snapshot_print_hh

#include <mirv/core/filter/forward_flow.hh>
#include <mirv/core/filter/symbol_flow.hh>
#include <mirv/core/filter/filter.hh>
#include <mirv/core/filter/action.hh>
#include <mirv/core/ir/node.hh>
#include <mirv/core/util/debug.hh>

namespace mirv {
   class PrintFilter
     : public Filter<Node<Base> > {
   private:
     typedef std::ostream Stream;
      typedef int Indent;
      const static int IndentFactor;
     Indent ind;
      Stream &out;
     bool JustLeft;

      // Entering each symbol
      class EnterDeclSymbolAction : public VisitSymbolAction {
      private:
         std::ostream &out;
         Indent &ind;
	bool &JustLeft;

      public:
	EnterDeclSymbolAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

	void visit(ptr<Symbol<Module> >::type sym);
	void visit(ptr<Symbol<Function> >::type sym);
	void visit(ptr<Symbol<Variable> >::type sym);
	void visit(ptr<Symbol<Type<Integral> > >::type sym);
	void visit(ptr<Symbol<Type<Floating> > >::type sym);
	void visit(ptr<Symbol<Type<Array> > >::type sym);
	void visit(ptr<Symbol<Type<Pointer> > >::type sym);
	void visit(ptr<Symbol<Type<FunctionType> > >::type sym);
      };

      class EnterDefSymbolAction : public VisitSymbolAction {
      private:
         std::ostream &out;
         Indent &ind;
	bool &JustLeft;

      public:
	EnterDefSymbolAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

         void visit(ptr<Symbol<Function> >::type sym);
         void visit(ptr<Symbol<Variable> >::type sym);
      };

      // Leaving each symbol
      class LeaveDeclSymbolAction : public VisitSymbolAction {
      private:
         Stream &out;
         Indent &ind;
	bool &JustLeft;

      public:
	LeaveDeclSymbolAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

	void visit(ptr<Symbol<Base> >::type sym) {
	  if (!JustLeft) {
	    out << "\n";
	  }
	  JustLeft = true;
	}
      };

      class LeaveDefSymbolAction : public VisitSymbolAction {
      private:
         Stream &out;
         Indent &ind;
	bool &JustLeft;

      public:
	LeaveDefSymbolAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

	void visit(ptr<Symbol<Variable> >::type sym) {
	  if (!JustLeft) {
	    out << "\n";
	  }
	  JustLeft = true;
	}
	void visit(ptr<Symbol<Module> >::type sym);
	void visit(ptr<Symbol<Function> >::type sym);
      };

      // Entering each statement
      class EnterAction : public VisitStatementAction {
      private:
         std::ostream &out;
         Indent &ind;
	bool &JustLeft;

      public:
	EnterAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

         void visit(ptr<Statement<Block> >::type stmt);
         void visit(ptr<Statement<IfThen> >::type stmt);
         void visit(ptr<Statement<IfElse> >::type stmt);
         void visit(ptr<Statement<While> >::type stmt);
         void visit(ptr<Statement<DoWhile> >::type stmt);
         void visit(ptr<Statement<Switch> >::type stmt);
         void visit(ptr<Statement<Case> >::type stmt);
         void visit(ptr<Statement<CaseBlock> >::type stmt);
         void visit(ptr<Statement<Before> >::type stmt);
         void visit(ptr<Statement<After> >::type stmt);
         void visit(ptr<Statement<Goto> >::type stmt);
         void visit(ptr<Statement<Return> >::type stmt);
         void visit(ptr<Statement<Assignment> >::type stmt);
      };

      // After processing each statement's expression child
      class AfterStmtExprAction : public VisitStatementAction {
      private:
         Stream &out;
         Indent &ind;

      public:
         AfterStmtExprAction(Stream &o, Indent &i)
               : out(o), ind(i) {}
         void visit(ptr<Statement<IfThen> >::type stmt) {
            ind -= IndentFactor;
         }

         void visit(ptr<Statement<IfElse> >::type stmt) {
            ind -= IndentFactor;
         }

         void visit(ptr<Statement<While> >::type stmt) {
            ind -= IndentFactor;
         }

         void visit(ptr<Statement<DoWhile> >::type stmt) {
            ind -= IndentFactor;
         }

         void visit(ptr<Statement<Switch> >::type stmt) {
            ind -= IndentFactor;
         }
      };

      // Leaving each statement
      class LeaveAction : public VisitStatementAction {
      private:
         Stream &out;
         Indent &ind;
	bool &JustLeft;

      public:
	LeaveAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

	void visit(ptr<Statement<Base> >::type stmt) {
	  if (!JustLeft) {
	    out << "\n";
	  }
	  ind -= IndentFactor;
	  JustLeft = true;
	}
	void visit(ptr<Statement<Block> >::type stmt);
	void visit(ptr<Statement<Return> >::type stmt);
      };

      // Entering each expression
      class EnterExprAction : public VisitExpressionAction {
      private:
         Stream &out;
         Indent &ind;
	bool &JustLeft;

      public:
	EnterExprAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

         void visit(ptr<Expression<Add> >::type expr);
         void visit(ptr<Expression<Subtract> >::type expr);
         void visit(ptr<Expression<Multiply> >::type expr);
         void visit(ptr<Expression<Divide> >::type expr);
         void visit(ptr<Expression<Modulus> >::type expr);
         void visit(ptr<Expression<Negate> >::type expr);
         void visit(ptr<Expression<LogicalAnd> >::type expr);
         void visit(ptr<Expression<LogicalOr> >::type expr);
         void visit(ptr<Expression<LogicalNot> >::type expr);
         void visit(ptr<Expression<BitwiseAnd> >::type expr);
         void visit(ptr<Expression<BitwiseOr> >::type expr);
         void visit(ptr<Expression<BitwiseComplement> >::type expr);
         void visit(ptr<Expression<LessThan> >::type expr);
         void visit(ptr<Expression<LessThanOrEqual> >::type expr);
         void visit(ptr<Expression<Equal> >::type expr);
         void visit(ptr<Expression<NotEqual> >::type expr);
         void visit(ptr<Expression<GreaterThan> >::type expr);
         void visit(ptr<Expression<GreaterThanOrEqual> >::type expr);
	void visit(ptr<Expression<Reference<Variable> > >::type expr);
      };

      // Leaving each expression
      class LeaveExprAction : public VisitExpressionAction {
      private:
         Stream &out;
         Indent &ind;
	bool &JustLeft;

      public:
	LeaveExprAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

	void visit(ptr<Expression<Base> >::type expr) {
	  if (!JustLeft) {
	    out << "\n";
	  }
	  JustLeft = true;
	}
	void visit(ptr<InnerExpression>::type expr) {
	  ind -= IndentFactor;
	  if (!JustLeft) {
	    out << "\n";
	  }
	  JustLeft = true;
	}
      };

      class PrintExpressionFlow
            : public ForwardExpressionFlow<
         EnterExprAction,
         LeaveExprAction,
         NullAction,
	 NullAction,
         NullAction
         > {
      private:
	typedef ForwardExpressionFlow<
         EnterExprAction,
         LeaveExprAction,
         NullAction,
	 NullAction,
         NullAction
	> BaseType;

      public:
	PrintExpressionFlow(const EnterExprAction &e,
			    const LeaveExprAction &l)
	  : BaseType(e, l, NullAction(), NullAction(), NullAction(), NullDataflow()) {}
      };

      class PrintFlow
            : public ForwardFlow<
	EnterAction,
	LeaveAction,
	NullAction,
	NullAction,
	NullAction,
	NullAction,
	NullAction,
	PrintExpressionFlow> {
	typedef ForwardFlow<
	  EnterAction,
	  LeaveAction,
	  NullAction,
	  NullAction,
	  NullAction,
	  NullAction,
	  NullAction,
	  PrintExpressionFlow> BaseType;

      public:
	PrintFlow(const EnterAction &e,
		 const LeaveAction &l,
		 const PrintExpressionFlow &expr)
	  : BaseType(e,
		     l,
		     NullAction(),
		     NullAction(),
		     NullAction(),
		     NullAction(),
		     NullAction(),
		     expr,
		     NullDataflow(),
		     NullDataflow::Confluence()) {}

	// We need to reverse the order in which we visit the
	// assignment operands.
	void visit(ptr<Statement<Assignment> >::type stmt) {
	  this->enter(stmt);

	  this->before_expression(stmt, stmt->get_left_expression());
	  stmt->get_left_expression()->accept(this->expression_flow());
	  this->after_expression(stmt, stmt->get_left_expression());

	  this->before_expression(stmt, stmt->get_right_expression());
	  stmt->get_right_expression()->accept(this->expression_flow());
	  this->after_expression(stmt, stmt->get_right_expression());

	  this->leave(stmt);
	}
      };

     class PrintDeclSymbolFlow : public SymbolFlow<
       EnterDeclSymbolAction,
       LeaveDeclSymbolAction,
       NullAction,
       NullAction,
       NullAction,
       NullStatementFlow> {
     private:
       typedef SymbolFlow<
       EnterDeclSymbolAction,
       LeaveDeclSymbolAction,
       NullAction,
       NullAction,
       NullAction,
       NullStatementFlow> BaseType;

     public:
       PrintDeclSymbolFlow(const EnterDeclSymbolAction &e,
			   const LeaveDeclSymbolAction &l)
	 : BaseType(e, l, NullAction(), NullAction(), NullAction(),
		    NullStatementFlow()) {}

       void visit(ptr<Symbol<Function> >::type sym) {
         this->enter(sym);
         this->leave(sym);
       }
     };

     class PrintDefSymbolFlow : public SymbolFlow<
       EnterDefSymbolAction,
       LeaveDefSymbolAction,
       NullAction,
       NullAction,
       NullAction,
       PrintFlow> {
     private:
       typedef SymbolFlow<
       EnterDefSymbolAction,
       LeaveDefSymbolAction,
       NullAction,
       NullAction,
       NullAction,
       PrintFlow> BaseType;

     public:
       PrintDefSymbolFlow(const EnterDefSymbolAction &e,
			  const LeaveDefSymbolAction &l,
			  const PrintFlow &p)
	 : BaseType(e, l, NullAction(), NullAction(), NullAction(), p) {}

       void visit(ptr<Symbol<Module> >::type sym) {
         this->enter(sym);
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
     };

   public:
      PrintFilter(Stream &o)
	: Filter<Node<Base> >(), ind(0), out(o) {}


      class indent {
      private:
         int val;

      public:
         indent(int ind)
               : val(ind) {};

         Stream &operator()(Stream &out) const {
	   check_invariant(val >= 0, "Indent underflow");
	   int i = val;
            while(i--) {
               out << " ";
            }
            return(out);
         }
      };

     void operator()(ptr<Node<Base> >::type node);
   };

  inline std::ostream &operator<<(std::ostream &out, const PrintFilter::indent &ind) {
    return ind(out);
  }
}

#endif
