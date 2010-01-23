#include <mirv/core/filter/expression_visitor.hh>

#include <mirv/core/ir/expression.hh>
#include <mirv/core/ir/arithmetic.hh>
#include <mirv/core/ir/logical.hh>
#include <mirv/core/ir/bitwise.hh>
#include <mirv/core/ir/reference.hh>
#include <mirv/core/util/cast.hh>

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
