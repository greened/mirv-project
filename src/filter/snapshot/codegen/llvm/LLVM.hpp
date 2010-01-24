#ifndef mirv_filter_snapshot_codegen_LLVM_hpp
#define mirv_filter_snapshot_codegen_LLVM_hpp

#include <mirv/core/filter/ForwardFlow.hpp>
#include <mirv/core/filter/Filter.hpp>
#include <mirv/core/filter/Action.hpp>
#include <mirv/core/ir/Node.hpp>

namespace mirv {
  /// This is a filter to translate from MIRV IR to LLVM IR.
  class LLVMCodegenFilter
    : public Filter<Node<Base> > {
  private:
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
