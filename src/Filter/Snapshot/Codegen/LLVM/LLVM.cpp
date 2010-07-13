#include "LLVM.hpp"

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Block> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<IfThen> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<IfElse> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<While> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<DoWhile> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Switch> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Case> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<CaseBlock> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Before> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<After> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Goto> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Return> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Assignment> >::type stmt)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<Add> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<Subtract> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<Multiply> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<Divide> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<Modulus> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<Negate> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalOr> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalNot> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<LessThan> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<Equal> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<NotEqual> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<GreaterThan> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::type expr)
  {
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::type expr)
  {
  }

  void LLVMCodegenFilter::operator()(ptr<Node<Base> >::type node)
  {
    //if (ptr<Statement<Base> >::type s = dyn_cast<Statement<Base> >(node)) {
    if (ptr<Statement<Base> >::type s = boost::dynamic_pointer_cast<Statement<Base> >(node)) {
      ptr<LLVMCodegenFlow>::type flow(new LLVMCodegenFlow());
      s->accept(*flow);
    }
    else if (ptr<Expression<Base> >::type e = boost::dynamic_pointer_cast<Expression<Base> >(node)) {
      ptr<LLVMCodegenExpressionFlow>::type flow(new LLVMCodegenExpressionFlow());
      e->accept(*flow);
    }
  }
}
