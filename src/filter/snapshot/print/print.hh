#ifndef mirv_filter_snapshot_print_hh
#define mirv_filter_snapshot_print_hh

#include <mirv/filter/flow.hh>
#include <mirv/filter/filter.hh>
#include <mirv/filter/action.hh>
#include <mirv/ir/node.hh>

namespace mirv {
   class PrintFilter
         : public Filter<BaseNode> {
   private:
     typedef std::ostream Stream;
      typedef int Indent;
      const int IndentFactor;

      class indent {
      private:
         int i;

      public:
         indent(int ind)
               : i(ind) {};
         template<typename Stream>
         std::ostream &operator()(std::ostream &out) const {
            while(i--) {
               out << " ";
            }
            return(out);
         }
      };

      indent ind;
      Stream &out;

      // Entering each statement
      class EnterAction
            : public VisitStatementAction<void> {
      private:
         std::ostream &out;
         Indent &ind;

      public:
         EnterAction(Stream &o, Indent &i)
               : out(o), indent(i) {}

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
      class AfterStmtExprAction
            : public VisitStatementAction<void> {
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
      class AfterAction
            : public VisitStatementAction<void> {
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
      class LeaveAction
            : public VisitStatementAction<void> {
      private:
         Stream &out;
         Indent &ind;

      public:
         LeaveAction(Stream &o, Indent &i)
               : out(o), ind(i) {}

         void visit(Statement<Block> &stmt) {
            indent -= IndentFactor;
            out << indent(ind) << "}";
         }
      };

      // Entering each expression
      class EnterExprAction
            : public VisitExpressionAction<void> {
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
      class LeaveExprAction
            : public VisitExpressionAction<void> {
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
      class AfterExprAction
            : public VisitExpressionAction<void> {
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
         AfterExprAction
         > {};

      typedef ForwardFlow<
         EnterAction,
         LeaveAction,
         NullAction,
         AfterAction
         PrintExpressionFlow> PrintFlow;

   public:
      PrintFilter(std::ostream &o)
	: Filter<BaseNode>(), IndentFactor(3), indent(), out(o) {}

      void operator()(BaseNode &node);
   };
}

#endif
