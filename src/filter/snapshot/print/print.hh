#ifndef MIRV_Filter_Snapshot_Print_hh
#define MIRV_filter_Snapshot_Print_hh

#include <mirv/filter/flow.hh>

namespace MIRV {
   class PrintFilter
         : public Filter<BaseNode> {
   private:
      typedef int Indent;
      const int IndentFactor = 3;

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
      std::ostream &out;

      // Entering each statement
      class EnterAction
            : public StatementAction<void> {
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
            : public StatementAction<void> {
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
            : public StatementAction<void> {
      private:
         Stream &out;
         Indent &ind;

      public:
         AfterAction(Stream &o, Indent &i)
               : out(o), ind(i) {}
         void visit(BaseStmt &stmt) {
            out << "\n";
         }
      };

      // Leaving each statement
      class LeaveAction
            : public StatementAction<void> {
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
            : public ExpressionAction<void> {
      private:
         Stream &out;
         Indent &ind;

      public:
         EnterExprAction(Stream &o, Indent &i)
               : out(o), ind(i) {}

         void visit(Expression<Add> &expr);
         void visit(Expression<Sub> &expr);
         void visit(Expression<Mult> &expr);
         void visit(Expression<Div> &expr);
         void visit(Expression<Mod> &expr);
         void visit(Expression<Neg> &expr);
         void visit(Expression<LogicalAnd> &expr);
         void visit(Expression<LogicalOr> &expr);
         void visit(Expression<LogicalNot> &expr);
         void visit(Expression<BitwiseAnd> &expr);
         void visit(Expression<BitwiseOr> &expr);
         void visit(Expression<Complement> &expr);
      };

      // Leaving each expression
      class LeaveExprAction
            : public ExpressionAction<void> {
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
            : public ExpressionAction<void> {
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
         BeforeExprAction,
         AfterExprAction
         > {};

      typedef ForwardFlow<
         EnterAction,
         LeaveAction,
         BeforeAction,
         AfterAction
         PrintExpressionFlow> PrintFlow;

   public:
      PrintFilter(std::ostream &o)
            : Filter<BaseNode>(), indent(), out(o) {}

      void operator()(BaseNode &node);
   };
}

#endif
