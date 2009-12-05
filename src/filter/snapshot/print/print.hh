#ifndef mirv_filter_snapshot_print_hh
#define mirv_filter_snapshot_print_hh

#include <mirv/core/filter/forward_flow.hh>
#include <mirv/core/filter/filter.hh>
#include <mirv/core/filter/action.hh>
#include <mirv/core/ir/node.hh>

namespace mirv {
   class PrintFilter
     : public Filter<Node<Base> > {
   private:
     typedef std::ostream Stream;
      typedef int Indent;
      const static int IndentFactor;
     Indent ind;
      Stream &out;

      // Entering each statement
      class EnterAction : public VisitStatementAction {
      private:
         std::ostream &out;
         Indent &ind;

      public:
         EnterAction(Stream &o, Indent &i)
               : out(o), ind(i) {}

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

      // After each statement's child
      class AfterAction : public VisitStatementAction {
      private:
         Stream &out;
         Indent &ind;

      public:
         AfterAction(Stream &o, Indent &i)
               : out(o), ind(i) {}
	void visit(ptr<Statement<Base> >::type stmt) {
            out << "\n";
         }
      };

      // Leaving each statement
      class LeaveAction : public VisitStatementAction {
      private:
         Stream &out;
         Indent &ind;

      public:
         LeaveAction(Stream &o, Indent &i)
               : out(o), ind(i) {}

	void visit(ptr<Statement<Block> >::type stmt);
      };

      // Entering each expression
      class EnterExprAction : public VisitExpressionAction {
      private:
         Stream &out;
         Indent &ind;

      public:
         EnterExprAction(Stream &o, Indent &i)
               : out(o), ind(i) {}

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
	void visit(ptr<Expression<Reference<Variable> > >::type expr);
      };

      // Leaving each expression
      class LeaveExprAction : public VisitExpressionAction {
      private:
         Stream &out;
         Indent &ind;

      public:
         LeaveExprAction(Stream &o, Indent &i)
               : out(o), ind(i) {}

         void visit(ptr<InnerExpression>::type expr) {
            ind -= IndentFactor;
         }
      };

      // After each expression's child
      class AfterExprAction : public VisitExpressionAction {
      private:
         Stream &out;
         Indent &ind;

      public:
         AfterExprAction(Stream &o, Indent &i)
               : out(o), ind(i) {}

	void visit(ptr<Expression<Base> >::type expr) {
            out << "\n";
         }
      };

      class PrintExpressionFlow
            : public ForwardExpressionFlow<
         EnterExprAction,
         LeaveExprAction,
         NullAction,
	 NullAction,
         AfterExprAction
         > {
      private:
	typedef ForwardExpressionFlow<
         EnterExprAction,
         LeaveExprAction,
         NullAction,
	 NullAction,
         AfterExprAction
	> BaseType;

      public:
	PrintExpressionFlow(const EnterExprAction &e,
			    const LeaveExprAction &l,
			    const AfterExprAction &a)
	  : BaseType(e, l, NullAction(), NullAction(), a, NullDataflow()) {}
      };

      typedef ForwardFlow<
         EnterAction,
         LeaveAction,
         NullAction,
	AfterAction,
	NullAction,
	NullAction,
	NullAction,
	PrintExpressionFlow> PrintFlow;

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
	   int i = val;
            while(i--) {
               out << " ";
            }
            return(out);
         }
      };

     void operator()(ptr<Node<Base> >::type node);
   };

  std::ostream &operator<<(std::ostream &out, const PrintFilter::indent &ind) {
    return ind(out);
  }
}

#endif
