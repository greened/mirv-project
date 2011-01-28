#include <mirv/Core/Filter/ConstExpressionVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/IR/Arithmetic.hpp>
#include <mirv/Core/IR/Logical.hpp>
#include <mirv/Core/IR/Bitwise.hpp>
#include <mirv/Core/IR/Reference.hpp>
#include <mirv/Core/IR/Relational.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/Variable.hpp>

namespace mirv {
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Base> >::const_type e) {}
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<InnerExpression>::const_type e) {
    ptr<InnerExpression::VisitorBaseType>::const_type p = fast_cast<const InnerExpression::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<InnerExpressionBase>::const_type e) {
    ptr<InnerExpressionBase::VisitorBaseType>::const_type p = fast_cast<const InnerExpressionBase::VisitorBaseType>(e);
    visit(static_cast<ptr<InnerExpressionBase::VisitorBaseType>::const_type>(e));
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<LeafExpression>::const_type e) {
    ptr<LeafExpression::VisitorBaseType>::const_type p = fast_cast<const LeafExpression::VisitorBaseType>(e);
    visit(p);
  }
  // Establish an order for properties.  This coresponds to the
  // property tag ordering.
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Arithmetic> >::const_type e) {
    if (ptr<Expression<Unary> >::const_type ue =
	dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::const_type be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Arithmetic>::VisitorBaseType>::const_type p = fast_cast<const Expression<Arithmetic>::VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Logical> >::const_type e) {
    if (ptr<Expression<Unary> >::const_type ue =
	dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::const_type be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Logical>::VisitorBaseType>::const_type p = fast_cast<const Expression<Logical>::VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Bitwise> >::const_type e) {
    if (ptr<Expression<Unary> >::const_type ue =
	dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::const_type be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Bitwise>::VisitorBaseType>::const_type p = fast_cast<const Expression<Bitwise>::VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Relational> >::const_type e) {
    if (ptr<Expression<Unary> >::const_type ue =
	dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::const_type be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Relational>::VisitorBaseType>::const_type p = fast_cast<const Expression<Relational>::VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Commutative> >::const_type e) {
    if (ptr<Expression<Bitwise> >::const_type be =
	dyn_cast<const Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::const_type le =
	     dyn_cast<const Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::const_type ae =
	     dyn_cast<const Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::const_type ue =
	     dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::const_type be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Commutative>::VisitorBaseType>::const_type p = fast_cast<const Expression<Commutative>::VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Associative> >::const_type e) {
    if (ptr<Expression<Commutative> >::const_type ce =
	dyn_cast<const Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::const_type be =
	     dyn_cast<const Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::const_type le =
	     dyn_cast<const Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::const_type ae =
	     dyn_cast<const Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::const_type ue =
	     dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::const_type be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Associative>::VisitorBaseType>::const_type p = fast_cast<const Expression<Associative>::VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Transitive> >::const_type e) {
    if (ptr<Expression<Associative> >::const_type ase =
	dyn_cast<const Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> >::const_type ce =
	     dyn_cast<const Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::const_type be =
	     dyn_cast<const Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::const_type le =
	     dyn_cast<const Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::const_type ae =
	     dyn_cast<const Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::const_type ue =
	     dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::const_type be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Transitive>::VisitorBaseType>::const_type p = fast_cast<const Expression<Transitive>::VisitorBaseType>(e);
      visit(p);
    }
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Reflexive> >::const_type e) {
    if (ptr<Expression<Transitive> >::const_type te =
	dyn_cast<const Expression<Transitive> >(e)) {
      visit(te);
    }
    else if (ptr<Expression<Associative> >::const_type ase =
	dyn_cast<const Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> >::const_type ce =
	     dyn_cast<const Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::const_type be =
	     dyn_cast<const Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::const_type le =
	     dyn_cast<const Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::const_type ae =
	     dyn_cast<const Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::const_type ue =
	     dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::const_type be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Reflexive>::VisitorBaseType>::const_type p = fast_cast<const Expression<Reflexive>::VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Ref> >::const_type e) {
    if (ptr<Expression<Reflexive> >::const_type re =
	dyn_cast<const Expression<Reflexive> >(e)) {
      visit(re);
    }
    else if (ptr<Expression<Transitive> >::const_type te =
	dyn_cast<const Expression<Transitive> >(e)) {
      visit(te);
    }
    else if (ptr<Expression<Associative> >::const_type ase =
	dyn_cast<const Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> >::const_type ce =
	     dyn_cast<const Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::const_type be =
	     dyn_cast<const Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::const_type le =
	     dyn_cast<const Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::const_type ae =
	     dyn_cast<const Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::const_type ue =
	     dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::const_type be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      ptr<Expression<Ref>::VisitorBaseType>::const_type p = fast_cast<const Expression<Ref>::VisitorBaseType>(e);
      visit(p);
    }
  }

ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Unary> >::const_type e) {
  ptr<Expression<Unary>::VisitorBaseType>::const_type p = fast_cast<const Expression<Unary>::VisitorBaseType>(e);
  visit(p);
}
ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Binary> >::const_type e) {
  ptr<Expression<Binary>::VisitorBaseType>::const_type p = fast_cast<const Expression<Binary>::VisitorBaseType>(e);
  visit(p);
}

  // We always prefer to visit the properties because they give us
  // semantic information.
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Add> >::const_type e) {
    ptr<Expression<Add>::VisitorBaseType>::const_type p = fast_cast<const Expression<Add>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Subtract> >::const_type e) {
    ptr<Expression<Subtract>::VisitorBaseType>::const_type p = fast_cast<const Expression<Subtract>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Divide> >::const_type e) {
    ptr<Expression<Divide>::VisitorBaseType>::const_type p = fast_cast<const Expression<Divide>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Multiply> >::const_type e) {
    ptr<Expression<Multiply>::VisitorBaseType>::const_type p = fast_cast<const Expression<Multiply>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Modulus> >::const_type e) {
    ptr<Expression<Modulus>::VisitorBaseType>::const_type p = fast_cast<const Expression<Modulus>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Negate> >::const_type e) {
    ptr<Expression<Negate>::VisitorBaseType>::const_type p = fast_cast<const Expression<Negate>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::const_type e) {
    ptr<Expression<LogicalAnd>::VisitorBaseType>::const_type p = fast_cast<const Expression<LogicalAnd>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LogicalOr> >::const_type e) {
    ptr<Expression<LogicalOr>::VisitorBaseType>::const_type p = fast_cast<const Expression<LogicalOr>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LogicalNot> >::const_type e) {
    ptr<Expression<LogicalNot>::VisitorBaseType>::const_type p = fast_cast<const Expression<LogicalNot>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::const_type e) {
    ptr<Expression<BitwiseAnd>::VisitorBaseType>::const_type p = fast_cast<const Expression<BitwiseAnd>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::const_type e) {
    ptr<Expression<BitwiseOr>::VisitorBaseType>::const_type p = fast_cast<const Expression<BitwiseOr>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<BitwiseXor> >::const_type e) {
    ptr<Expression<BitwiseXor>::VisitorBaseType>::const_type p = fast_cast<const Expression<BitwiseXor>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::const_type e) {
    ptr<Expression<BitwiseComplement>::VisitorBaseType>::const_type p = fast_cast<const Expression<BitwiseComplement>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<ShiftLeft> >::const_type e) {
    ptr<Expression<ShiftLeft>::VisitorBaseType>::const_type p = fast_cast<const Expression<ShiftLeft>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<ArithmeticShiftRight> >::const_type e) {
    ptr<Expression<ArithmeticShiftRight>::VisitorBaseType>::const_type p = fast_cast<const Expression<ArithmeticShiftRight>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LogicalShiftRight> >::const_type e) {
    ptr<Expression<LogicalShiftRight>::VisitorBaseType>::const_type p = fast_cast<const Expression<LogicalShiftRight>::VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LessThan> >::const_type e) {
    ptr<Expression<LessThan>::VisitorBaseType>::const_type p = fast_cast<const Expression<LessThan>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::const_type e) {
    ptr<Expression<LessThanOrEqual>::VisitorBaseType>::const_type p = fast_cast<const Expression<LessThanOrEqual>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Equal> >::const_type e) {
    ptr<Expression<Equal>::VisitorBaseType>::const_type p = fast_cast<const Expression<Equal>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<NotEqual> >::const_type e) {
    ptr<Expression<NotEqual>::VisitorBaseType>::const_type p = fast_cast<const Expression<NotEqual>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<GreaterThan> >::const_type e) {
    ptr<Expression<GreaterThan>::VisitorBaseType>::const_type p = fast_cast<const Expression<GreaterThan>::VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::const_type e) {
    ptr<Expression<GreaterThanOrEqual>::VisitorBaseType>::const_type p = fast_cast<const Expression<GreaterThanOrEqual>::VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::const_type e) {
    ptr<Expression<Reference<Variable> >::VisitorBaseType>::const_type p = fast_cast<const Expression<Reference<Variable> >::VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Reference<Function> > >::const_type e) {
    ptr<Expression<Reference<Function> >::VisitorBaseType>::const_type p = fast_cast<const Expression<Reference<Function> >::VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Reference<Constant<Base> > > >::const_type e) {
    ptr<Expression<Reference<Constant<Base> > >::VisitorBaseType>::const_type p = fast_cast<const Expression<Reference<Constant<Base> > >::VisitorBaseType>(e);
    visit(p);
  }
}
