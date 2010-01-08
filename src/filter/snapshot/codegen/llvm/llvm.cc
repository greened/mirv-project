#include "llvm.hh"

#include <mirv/core/ir/ir.hh>
#include <mirv/core/util/cast.hh>

namespace mirv {
  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<Block> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<IfThen> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<IfElse> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<While> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<DoWhile> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<Switch> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<Case> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<CaseBlock> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<Before> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<After> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<Goto> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<Return> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterAction::visit(ptr<Statement<Assignment> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<Add> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<Subtract> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<Multiply> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<Divide> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<Modulus> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<Negate> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<LogicalAnd> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<LogicalOr> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<LogicalNot> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<BitwiseAnd> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<BitwiseOr> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<BitwiseComplement> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<LessThan> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<LessThanOrEqual> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<Equal> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<NotEqual> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<GreaterThan> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<GreaterThanOrEqual> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExprAction::visit(ptr<Expression<Reference<Variable> > >::type expr)
  {
  }

  void LLVMCodegenFilter::operator()(ptr<Node<Base> >::type node)
  {
    //if (ptr<Statement<Base> >::type s = dyn_cast<Statement<Base> >(node)) {
    if (ptr<Statement<Base> >::type s = boost::dynamic_pointer_cast<Statement<Base> >(node)) {
      ptr<StatementVisitor>::type flow(new LLVMCodegenFlow(EnterAction(),
							   LLVMCodegenExpressionFlow(EnterExprAction())));
      s->accept(*flow);
    }
    else if (ptr<Expression<Base> >::type e = boost::dynamic_pointer_cast<Expression<Base> >(node)) {
      ptr<ExpressionVisitor>::type flow(new LLVMCodegenExpressionFlow(EnterExprAction()));
      e->accept(*flow);
    }
  }
}
