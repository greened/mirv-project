#ifndef mirv_Filter_Snapshot_Print_hpp
#define mirv_Filter_Snapshot_Print_hpp

#include <mirv/Core/Filter/ForwardFlow.hpp>
#include <mirv/Core/Filter/SymbolFlow.hpp>
#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/Utility/Debug.hpp>

namespace mirv {
  /// This is a filter to print MIRV IR in textual form.
   class PrintFilter
     : public Filter<Node<Base> > {
   private:
     typedef std::ostream Stream;
     /// The current indent level.
      typedef int Indent;
     /// The factor by which to increase the indent at deeper nesting
     /// levels.
      const static int IndentFactor;
     /// The indent manipulator.
     Indent ind;
     /// The stream to dump to.
     Stream &out;
     /// This indicates whether we just left an IR subtree.  If so, we
     /// want to suppress printing of a newline.
     bool JustLeft;

      /// Entering each symbol
      class EnterDeclSymbolAction : public VisitSymbolAction {
      private:
     /// The stream to dump to.
         std::ostream &out;
	/// The indent manipulator.
         Indent &ind;
     /// This indicates whether we just left an IR subtree.  If so, we
     /// want to suppress printing of a newline.
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
	/// The stream to dump to.
         std::ostream &out;
	/// The indent manipulator.
         Indent &ind;
     /// This indicates whether we just left an IR subtree.  If so, we
     /// want to suppress printing of a newline.
	bool &JustLeft;

      public:
	EnterDefSymbolAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

         void visit(ptr<Symbol<Function> >::type sym);
         void visit(ptr<Symbol<Variable> >::type sym);
      };

      /// Leaving each symbol declaration.
      class LeaveDeclSymbolAction : public VisitSymbolAction {
      private:
	/// The stream to dump to.
	Stream &out;
	/// The indent manipulator.
         Indent &ind;
     /// This indicates whether we just left an IR subtree.  If so, we
     /// want to suppress printing of a newline.
	bool &JustLeft;

      public:
	LeaveDeclSymbolAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

	/// Print the final newline after each symbol declaration.
	void visit(ptr<Symbol<Base> >::type sym) {
	  if (!JustLeft) {
	    out << "\n";
	  }
	  JustLeft = true;
	}
      };

     /// Leaving each symbol definition.
      class LeaveDefSymbolAction : public VisitSymbolAction {
      private:
	/// The stream to dump to.
         Stream &out;
	/// The indent manipulator.
         Indent &ind;
     /// This indicates whether we just left an IR subtree.  If so, we
     /// want to suppress printing of a newline.
	bool &JustLeft;

      public:
	LeaveDefSymbolAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

	/// Print the final newline after each symbol definition.
	void visit(ptr<Symbol<Variable> >::type sym) {
	  if (!JustLeft) {
	    out << "\n";
	  }
	  JustLeft = true;
	}
	void visit(ptr<Symbol<Module> >::type sym);
	void visit(ptr<Symbol<Function> >::type sym);
      };

     /// Entering each statement
      class EnterAction : public VisitStatementAction {
      private:
	/// The stream to dump to.
         std::ostream &out;
	/// The indent manipulator.
         Indent &ind;
     /// This indicates whether we just left an IR subtree.  If so, we
     /// want to suppress printing of a newline.
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

     /// After processing each statement's expression child
      class AfterStmtExprAction : public VisitStatementAction {
      private:
	/// The stream to dump to.
         Stream &out;
	/// The indent manipulator.
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

     /// Leaving each statement
      class LeaveAction : public VisitStatementAction {
      private:
	/// The stream to dump to.
         Stream &out;
	/// The indent manipulator.
         Indent &ind;
     /// This indicates whether we just left an IR subtree.  If so, we
     /// want to suppress printing of a newline.
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

     /// Entering each expression
      class EnterExprAction : public VisitExpressionAction {
      private:
	/// The stream to dump to.
         Stream &out;
	/// The indent manipulator.
         Indent &ind;
     /// This indicates whether we just left an IR subtree.  If so, we
     /// want to suppress printing of a newline.
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

     /// Leaving each expression
      class LeaveExprAction : public VisitExpressionAction {
      private:
	/// The stream to dump to.
	Stream &out;
	/// The indent manipulator.
         Indent &ind;
     /// This indicates whether we just left an IR subtree.  If so, we
     /// want to suppress printing of a newline.
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

     /// This is the flow to print expressions.
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

     /// This is the flow to print statements.
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

	  this->beforeExpression(stmt, stmt->getLeftExpression());
	  stmt->getLeftExpression()->accept(this->expressionFlow());
	  this->afterExpression(stmt, stmt->getLeftExpression());

	  this->beforeExpression(stmt, stmt->getRightExpression());
	  stmt->getRightExpression()->accept(this->expressionFlow());
	  this->afterExpression(stmt, stmt->getRightExpression());

	  this->leave(stmt);
	}
      };

     /// This is the flow to print symbol declarations.  It prints the module definition since there is no such thing as a module declaration.
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

       // We just want the function declaration at this point so don't
       // visit children.
       void visit(ptr<Symbol<Function> >::type sym) {
         this->enter(sym);
         this->leave(sym);
       }
     };

     /// This is the flow to print symbol defintions.
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

       /// We only want to visit functions here since we already
       /// declared module-level types and variables.
       void visit(ptr<Symbol<Module> >::type sym) {
         this->enter(sym);
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
     };

   public:
      PrintFilter(Stream &o)
	: Filter<Node<Base> >(), ind(0), out(o) {}

     /// This is an iostream manipulator to print a specified number
     /// of spaces for indentation.
     class indent {
      private:
       /// The number of spaces to print.
         int val;

      public:
         indent(int ind)
               : val(ind) {};

       /// Print the spafces specified by val.
         Stream &operator()(Stream &out) const {
	   checkInvariant(val >= 0, "Indent underflow");
	   int i = val;
            while(i--) {
               out << " ";
            }
            return(out);
         }
      };

     void operator()(ptr<Node<Base> >::type node);
   };

  /// This is the stream operator for the indent iostream manipulator.
  inline std::ostream &operator<<(std::ostream &out, const PrintFilter::indent &ind) {
    return ind(out);
  }
}

#endif
