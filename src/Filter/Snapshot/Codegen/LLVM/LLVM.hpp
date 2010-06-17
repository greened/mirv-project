#ifndef mirv_Filter_Snapshot_Codegen_LLVM_hpp
#define mirv_Filter_Snapshot_Codegen_LLVM_hpp

#include <mirv/Core/Filter/ForwardFlow.hpp>
#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/IR/Node.hpp>

namespace mirv {
  /// This is a filter to translate from MIRV IR to LLVM IR.
  class LLVMCodegenFilter
      : public Filter<Node<Base> > {
  private:
    /// Entering each symbol
    class EnterDeclSymbolAction : public VisitSymbolAction {
    public:
      EnterDeclSymbolAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

      void visit(ptr<Symbol<Module> >::type sym);
      void visit(ptr<Symbol<Function> >::type sym);
      void visit(ptr<Symbol<Variable> >::type sym);
      void visit(ptr<Symbol<Type<TypeBase> > >::type sym);
      void visit(ptr<Symbol<Type<StructType> > >::type sym);
    };

    class EnterDefSymbolAction : public VisitSymbolAction {
    public:
      EnterDefSymbolAction(Stream &o, Indent &i, bool &j)
	  : out(o), ind(i), JustLeft(j) {}

      void visit(ptr<Symbol<Function> >::type sym);
      void visit(ptr<Symbol<Variable> >::type sym);
    };

    /// Leaving each symbol declaration.
    class LeaveDeclSymbolAction : public VisitSymbolAction {
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

    public:
      EnterAction() {}

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

    /// Entering each expression
    class EnterExprAction : public VisitExpressionAction {
    private:

    public:
      EnterExprAction() {}

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

    /// This is the flow for translating expressions.
    class LLVMCodegenExpressionFlow
        : public ForwardExpressionFlow<
      EnterExprAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction
      > {
    private:
      typedef ForwardExpressionFlow<
      EnterExprAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction
      > BaseType;

    public:
      LLVMCodegenExpressionFlow(const EnterExprAction &e)
          : BaseType(e, NullAction(), NullAction(), NullAction(),
                     NullAction(), NullDataflow()) {}
    };

    /// This is the flow for translating statements.
    class LLVMCodegenFlow : public ForwardFlow<
      EnterAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction,
      LLVMCodegenExpressionFlow> {
      typedef ForwardFlow<
	EnterAction,
	NullAction,
	NullAction,
	NullAction,
	NullAction,
	NullAction,
	NullAction,
	LLVMCodegenExpressionFlow> BaseType;

    public:
      LLVMCodegenFlow(const EnterAction &e,
		      const LLVMCodegenExpressionFlow &expr)
          : BaseType(e,
                     NullAction(),
                     NullAction(),
                     NullAction(),
                     NullAction(),
                     NullAction(),
                     NullAction(),
                     expr,
                     NullDataflow(),
                     NullDataflow::Confluence()) {}
    };

  public:
    LLVMCodegenFilter(void) : Filter<Node<Base> >() {}

    /// Translate an IR tree.
    void operator()(ptr<Node<Base> >::type node);
  };
}

#endif
