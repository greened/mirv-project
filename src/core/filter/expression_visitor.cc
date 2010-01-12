#include <mirv/core/filter/expression_visitor.hh>
#include <mirv/core/util/cast.hh>

#include <mirv/core/ir/expression.hh>
#include <mirv/core/ir/arithmetic.hh>
#include <mirv/core/ir/logical.hh>
#include <mirv/core/ir/bitwise.hh>
#include <mirv/core/ir/reference.hh>

namespace mirv {
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Base> >::type e) {}
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<InnerExpression>::type e) {
    ptr<InnerExpression::visitor_base_type>::type p = fast_cast<InnerExpression::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<InnerExpressionBase>::type e) {
    ptr<InnerExpressionBase::visitor_base_type>::type p = fast_cast<InnerExpressionBase::visitor_base_type>(e);
    visit(static_cast<ptr<InnerExpressionBase::visitor_base_type>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<LeafExpression>::type e) {
    ptr<LeafExpression::visitor_base_type>::type p = fast_cast<LeafExpression::visitor_base_type>(e);
    visit(p);
  }
  // Establish an order for properties.  This coresponds to the
  // property tag ordering.
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Arithmetic> >::type e) {
    if (ptr<Expression<Unary> >::type ue =
	dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Arithmetic>::visitor_base_type>::type p = fast_cast<Expression<Arithmetic>::visitor_base_type>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Logical> >::type e) {
    if (ptr<Expression<Unary> >::type ue =
	dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Logical>::visitor_base_type>::type p = fast_cast<Expression<Logical>::visitor_base_type>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Bitwise> >::type e) {
    if (ptr<Expression<Unary> >::type ue =
	dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Bitwise>::visitor_base_type>::type p = fast_cast<Expression<Bitwise>::visitor_base_type>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Relational> >::type e) {
    if (ptr<Expression<Unary> >::type ue =
	dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Relational>::visitor_base_type>::type p = fast_cast<Expression<Relational>::visitor_base_type>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Commutative> >::type e) {
    if (ptr<Expression<Bitwise> >::type be =
	dyn_cast<Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::type le =
	     dyn_cast<Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::type ae =
	     dyn_cast<Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::type ue =
	     dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Commutative>::visitor_base_type>::type p = fast_cast<Expression<Commutative>::visitor_base_type>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Associative> >::type e) {
    if (ptr<Expression<Commutative> >::type ce =
	dyn_cast<Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::type be =
	     dyn_cast<Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::type le =
	     dyn_cast<Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::type ae =
	     dyn_cast<Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::type ue =
	     dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Associative>::visitor_base_type>::type p = fast_cast<Expression<Associative>::visitor_base_type>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Transitive> >::type e) {
    if (ptr<Expression<Associative> >::type ase =
	dyn_cast<Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> >::type ce =
	     dyn_cast<Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::type be =
	     dyn_cast<Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::type le =
	     dyn_cast<Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::type ae =
	     dyn_cast<Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::type ue =
	     dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Transitive>::visitor_base_type>::type p = fast_cast<Expression<Transitive>::visitor_base_type>(e);
      visit(p);
    }
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reflexive> >::type e) {
    if (ptr<Expression<Transitive> >::type te =
	dyn_cast<Expression<Transitive> >(e)) {
      visit(te);
    }
    else if (ptr<Expression<Associative> >::type ase =
	dyn_cast<Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> >::type ce =
	     dyn_cast<Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::type be =
	     dyn_cast<Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::type le =
	     dyn_cast<Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::type ae =
	     dyn_cast<Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::type ue =
	     dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Reflexive>::visitor_base_type>::type p = fast_cast<Expression<Reflexive>::visitor_base_type>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Ref> >::type e) {
    if (ptr<Expression<Reflexive> >::type re =
	dyn_cast<Expression<Reflexive> >(e)) {
      visit(re);
    }
    else if (ptr<Expression<Transitive> >::type te =
	dyn_cast<Expression<Transitive> >(e)) {
      visit(te);
    }
    else if (ptr<Expression<Associative> >::type ase =
	dyn_cast<Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> >::type ce =
	     dyn_cast<Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::type be =
	     dyn_cast<Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::type le =
	     dyn_cast<Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::type ae =
	     dyn_cast<Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::type ue =
	     dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Ref>::visitor_base_type>::type p = fast_cast<Expression<Ref>::visitor_base_type>(e);
      visit(p);
    }
  }

ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Unary> >::type e) {
  ptr<Expression<Unary>::visitor_base_type>::type p = fast_cast<Expression<Unary>::visitor_base_type>(e);
  visit(p);
}
ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Binary> >::type e) {
  ptr<Expression<Binary>::visitor_base_type>::type p = fast_cast<Expression<Binary>::visitor_base_type>(e);
  visit(p);
}

  // We always prefer to visit the properties because they give us
  // semantic information.
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Add> >::type e) {
    ptr<Expression<Add>::visitor_base_type>::type p = fast_cast<Expression<Add>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Subtract> >::type e) {
    ptr<Expression<Subtract>::visitor_base_type>::type p = fast_cast<Expression<Subtract>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Divide> >::type e) {
    ptr<Expression<Divide>::visitor_base_type>::type p = fast_cast<Expression<Divide>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Multiply> >::type e) {
    ptr<Expression<Multiply>::visitor_base_type>::type p = fast_cast<Expression<Multiply>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Modulus> >::type e) {
    ptr<Expression<Modulus>::visitor_base_type>::type p = fast_cast<Expression<Modulus>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Negate> >::type e) {
    ptr<Expression<Negate>::visitor_base_type>::type p = fast_cast<Expression<Negate>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::type e) {
    ptr<Expression<LogicalAnd>::visitor_base_type>::type p = fast_cast<Expression<LogicalAnd>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalOr> >::type e) {
    ptr<Expression<LogicalOr>::visitor_base_type>::type p = fast_cast<Expression<LogicalOr>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalNot> >::type e) {
    ptr<Expression<LogicalNot>::visitor_base_type>::type p = fast_cast<Expression<LogicalNot>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::type e) {
    ptr<Expression<BitwiseAnd>::visitor_base_type>::type p = fast_cast<Expression<BitwiseAnd>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::type e) {
    ptr<Expression<BitwiseOr>::visitor_base_type>::type p = fast_cast<Expression<BitwiseOr>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::type e) {
    ptr<Expression<BitwiseComplement>::visitor_base_type>::type p = fast_cast<Expression<BitwiseComplement>::visitor_base_type>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LessThan> >::type e) {
    ptr<Expression<LessThan>::visitor_base_type>::type p = fast_cast<Expression<LessThan>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::type e) {
    ptr<Expression<LessThanOrEqual>::visitor_base_type>::type p = fast_cast<Expression<LessThanOrEqual>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Equal> >::type e) {
    ptr<Expression<Equal>::visitor_base_type>::type p = fast_cast<Expression<Equal>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<NotEqual> >::type e) {
    ptr<Expression<NotEqual>::visitor_base_type>::type p = fast_cast<Expression<NotEqual>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<GreaterThan> >::type e) {
    ptr<Expression<GreaterThan>::visitor_base_type>::type p = fast_cast<Expression<GreaterThan>::visitor_base_type>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::type e) {
    ptr<Expression<GreaterThanOrEqual>::visitor_base_type>::type p = fast_cast<Expression<GreaterThanOrEqual>::visitor_base_type>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::type e) {
    ptr<Expression<Reference<Variable> >::visitor_base_type>::type p = fast_cast<Expression<Reference<Variable> >::visitor_base_type>(e);
    visit(p);
  }
}
