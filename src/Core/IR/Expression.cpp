#include <mirv/Core/Filter/ExpressionVisitor.hpp>

#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Logical.hpp>
#include <mirv/Core/IR/Bitwise.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
  void Expression<Base>::accept(ExpressionVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<Expression<Base> >::type p = safe_cast<Expression<Base> >(pn);
    V.visit(p);
  }

  void Expression<Inner<detail::InnerExpressionTraits> >::accept(ExpressionVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<Expression<Base> >::type p = safe_cast<Expression<Base> >(pn);
    V.visit(p);
  }

  void InnerExpression::accept(ExpressionVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<InnerExpression>::type p = safe_cast<InnerExpression>(pn);
    V.visit(p);
  }

  void LeafExpression::accept(ExpressionVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<LeafExpression>::type p = safe_cast<LeafExpression>(pn);
    V.visit(p);
  }

  void Unary::Interface::accept(ExpressionVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<Expression<Unary> >::type p = safe_cast<Expression<Unary> >(pn);
    V.visit(p);
  }

  void Binary::Interface::accept(ExpressionVisitor &V)
  {
    ptr<Node<Base> >::type pn = shared_from_this();
    ptr<Expression<Binary> >::type p = safe_cast<Expression<Binary> >(pn);
    V.visit(p);
  }
}
