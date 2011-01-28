#include <mirv/Core/Filter/ConstExpressionVisitor.hpp>
#include <mirv/Core/Filter/ExpressionVisitor.hpp>
#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Logical.hpp>
#include <mirv/Core/IR/Bitwise.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Visitable.ipp>

namespace mirv {
  void
  Visitable<
    Expression<Base>,
    ExpressionVisitor
    >::accept(ExpressionVisitor &V)
  {
    error("Expression<Base>::accept called");
  }
  void
  ConstVisitable<
    Expression<Base>,
    ConstExpressionVisitor,
    ExpressionVisitor
    >::accept(ConstExpressionVisitor &V) const
  {
    error("Expression<Base>::accept called");
  }

  template
  void
  Visitable<InnerExpression, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<InnerExpression, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<InnerExpressionBase, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<InnerExpressionBase, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<LeafExpression, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<LeafExpression, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Arithmetic>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Arithmetic>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Bitwise>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Bitwise>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Logical>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Logical>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Ref>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Ref>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Relational>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Relational>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Associative>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Associative>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Commutative>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Commutative>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Reflexive>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Reflexive>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Transitive>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Transitive>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Unary>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Unary>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Binary>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Binary>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Add>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Add>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Subtract>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Subtract>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Multiply>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Multiply>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Divide>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Divide>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Modulus>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Modulus>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Negate>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Negate>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<LogicalAnd>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<LogicalAnd>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<LogicalOr>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<LogicalOr>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<LogicalNot>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<LogicalNot>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<BitwiseAnd>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<BitwiseAnd>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<BitwiseOr>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<BitwiseOr>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<BitwiseXor>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<BitwiseXor>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<BitwiseComplement>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<BitwiseComplement>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<ShiftLeft>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<ShiftLeft>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<ArithmeticShiftRight>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<ArithmeticShiftRight>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<LogicalShiftRight>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<LogicalShiftRight>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<LessThan>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<LessThan>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<LessThanOrEqual>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<LessThanOrEqual>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Equal>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Equal>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<GreaterThanOrEqual>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<GreaterThanOrEqual>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<GreaterThan>, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<GreaterThan>, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Reference<Function> >, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Reference<Function> >, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Reference<Variable> >, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Reference<Variable> >, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;

  template
  void
  Visitable<Expression<Reference<Constant<Base> > >, ExpressionVisitor>::accept(ExpressionVisitor &);
  template
  void
  ConstVisitable<Expression<Reference<Constant<Base> > >, ConstExpressionVisitor, ExpressionVisitor>::accept(ConstExpressionVisitor &) const;
}
