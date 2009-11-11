#ifndef mirv_filter_snapshot_print_hh
#define mirv_filter_snapshot_print_hh

#include <mirv/filter/forward_flow.hh>
#include <mirv/filter/filter.hh>
#include <mirv/filter/action.hh>
#include <mirv/ir/node.hh>

namespace mirv {
   class PrintFilter
         : public Filter<BaseNode> {
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

         void visit(Statement<Block> &stmt);
         void visit(Statement<IfThen> &stmt);
         void visit(Statement<IfElse> &stmt);
         void visit(Statement<While> &stmt);
         void visit(Statement<DoWhile> &stmt);
         void visit(Statement<Switch> &stmt);
         void visit(Statement<Case> &stmt);
         void visit(Statement<CaseBlock> &stmt);
         void visit(Statement<Before> &stmt);
         void visit(Statement<After> &stmt);
         void visit(Statement<Goto> &stmt);
         void visit(Statement<Return> &stmt);
      };

      // After processing each statement's expression child
      class AfterStmtExprAction : public VisitStatementAction {
      private:
         Stream &out;
         Indent &ind;

      public:
         AfterStmtExprAction(Stream &o, Indent &i)
               : out(o), ind(i) {}
         void visit(Statement<IfThen> &stmt) {
            ind -= IndentFactor;
         }

         void visit(Statement<IfElse> &stmt) {
            ind -= IndentFactor;
         }

         void visit(Statement<While> &stmt) {
            ind -= IndentFactor;
         }

         void visit(Statement<DoWhile> &stmt) {
            ind -= IndentFactor;
         }

         void visit(Statement<Switch> &stmt) {
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
         void visit(BaseStatement &stmt) {
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

	void visit(Statement<Block> &stmt);
      };

      // Entering each expression
      class EnterExprAction : public VisitExpressionAction {
      private:
         Stream &out;
         Indent &ind;

      public:
         EnterExprAction(Stream &o, Indent &i)
               : out(o), ind(i) {}

         void visit(Expression<Add> &expr);
         void visit(Expression<Subtract> &expr);
         void visit(Expression<Multiply> &expr);
         void visit(Expression<Divide> &expr);
         void visit(Expression<Modulus> &expr);
         void visit(Expression<Negate> &expr);
         void visit(Expression<LogicalAnd> &expr);
         void visit(Expression<LogicalOr> &expr);
         void visit(Expression<LogicalNot> &expr);
         void visit(Expression<BitwiseAnd> &expr);
         void visit(Expression<BitwiseOr> &expr);
         void visit(Expression<BitwiseComplement> &expr);
      };

      // Leaving each expression
      class LeaveExprAction : public VisitExpressionAction {
      private:
         Stream &out;
         Indent &ind;

      public:
         LeaveExprAction(Stream &o, Indent &i)
               : out(o), ind(i) {}

         void visit(InnerExpression &expr) {
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

         void visit(BaseExpression &expr) {
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
	: Filter<BaseNode>(), ind(0), out(o) {}


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

      void operator()(BaseNode &node);
   };

  std::ostream &operator<<(std::ostream &out, const PrintFilter::indent &ind) {
    return ind(out);
  }
}

#endif
