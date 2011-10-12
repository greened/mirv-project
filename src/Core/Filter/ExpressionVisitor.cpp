#include <mirv/Core/Filter/ExpressionVisitor.hpp>
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
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Base> >::type e) {}
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<InnerExpression>::type e) {
    ptr<InnerExpression::VisitorBaseType>::type p = fast_cast<InnerExpression::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<InnerExpressionBase>::type e) {
    ptr<InnerExpressionBase::VisitorBaseType>::type p = fast_cast<InnerExpressionBase::VisitorBaseType>(e);
    visit(static_cast<ptr<InnerExpressionBase::VisitorBaseType>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<LeafExpression>::type e) {
    ptr<LeafExpression::VisitorBaseType>::type p = fast_cast<LeafExpression::VisitorBaseType>(e);
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
      ptr<Expression<Arithmetic>::VisitorBaseType>::type p = fast_cast<Expression<Arithmetic>::VisitorBaseType>(e);
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
      ptr<Expression<Logical>::VisitorBaseType>::type p = fast_cast<Expression<Logical>::VisitorBaseType>(e);
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
      ptr<Expression<Bitwise>::VisitorBaseType>::type p = fast_cast<Expression<Bitwise>::VisitorBaseType>(e);
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
      ptr<Expression<Relational>::VisitorBaseType>::type p = fast_cast<Expression<Relational>::VisitorBaseType>(e);
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
      ptr<Expression<Commutative>::VisitorBaseType>::type p = fast_cast<Expression<Commutative>::VisitorBaseType>(e);
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
      ptr<Expression<Associative>::VisitorBaseType>::type p = fast_cast<Expression<Associative>::VisitorBaseType>(e);
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
      ptr<Expression<Transitive>::VisitorBaseType>::type p = fast_cast<Expression<Transitive>::VisitorBaseType>(e);
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
      ptr<Expression<Reflexive>::VisitorBaseType>::type p = fast_cast<Expression<Reflexive>::VisitorBaseType>(e);
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
      ptr<Expression<Ref>::VisitorBaseType>::type p = fast_cast<Expression<Ref>::VisitorBaseType>(e);
      visit(p);
    }
  }

ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Unary> >::type e) {
  ptr<Expression<Unary>::VisitorBaseType>::type p = fast_cast<Expression<Unary>::VisitorBaseType>(e);
  visit(p);
}
ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Binary> >::type e) {
  ptr<Expression<Binary>::VisitorBaseType>::type p = fast_cast<Expression<Binary>::VisitorBaseType>(e);
  visit(p);
}

  // We always prefer to visit the properties because they give us
  // semantic information.
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Add> >::type e) {
    ptr<Expression<Add>::VisitorBaseType>::type p = fast_cast<Expression<Add>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Subtract> >::type e) {
    ptr<Expression<Subtract>::VisitorBaseType>::type p = fast_cast<Expression<Subtract>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Divide> >::type e) {
    ptr<Expression<Divide>::VisitorBaseType>::type p = fast_cast<Expression<Divide>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Multiply> >::type e) {
    ptr<Expression<Multiply>::VisitorBaseType>::type p = fast_cast<Expression<Multiply>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Modulus> >::type e) {
    ptr<Expression<Modulus>::VisitorBaseType>::type p = fast_cast<Expression<Modulus>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Negate> >::type e) {
    ptr<Expression<Negate>::VisitorBaseType>::type p = fast_cast<Expression<Negate>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::type e) {
    ptr<Expression<LogicalAnd>::VisitorBaseType>::type p = fast_cast<Expression<LogicalAnd>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalOr> >::type e) {
    ptr<Expression<LogicalOr>::VisitorBaseType>::type p = fast_cast<Expression<LogicalOr>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalNot> >::type e) {
    ptr<Expression<LogicalNot>::VisitorBaseType>::type p = fast_cast<Expression<LogicalNot>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::type e) {
    ptr<Expression<BitwiseAnd>::VisitorBaseType>::type p = fast_cast<Expression<BitwiseAnd>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::type e) {
    ptr<Expression<BitwiseOr>::VisitorBaseType>::type p = fast_cast<Expression<BitwiseOr>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseXor> >::type e) {
    ptr<Expression<BitwiseXor>::VisitorBaseType>::type p = fast_cast<Expression<BitwiseXor>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::type e) {
    ptr<Expression<BitwiseComplement>::VisitorBaseType>::type p = fast_cast<Expression<BitwiseComplement>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<ShiftLeft> >::type e) {
    ptr<Expression<ShiftLeft>::VisitorBaseType>::type p = fast_cast<Expression<ShiftLeft>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<ArithmeticShiftRight> >::type e) {
    ptr<Expression<ArithmeticShiftRight>::VisitorBaseType>::type p = fast_cast<Expression<ArithmeticShiftRight>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalShiftRight> >::type e) {
    ptr<Expression<LogicalShiftRight>::VisitorBaseType>::type p = fast_cast<Expression<LogicalShiftRight>::VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LessThan> >::type e) {
    ptr<Expression<LessThan>::VisitorBaseType>::type p = fast_cast<Expression<LessThan>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::type e) {
    ptr<Expression<LessThanOrEqual>::VisitorBaseType>::type p = fast_cast<Expression<LessThanOrEqual>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Equal> >::type e) {
    ptr<Expression<Equal>::VisitorBaseType>::type p = fast_cast<Expression<Equal>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<NotEqual> >::type e) {
    ptr<Expression<NotEqual>::VisitorBaseType>::type p = fast_cast<Expression<NotEqual>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<GreaterThan> >::type e) {
    ptr<Expression<GreaterThan>::VisitorBaseType>::type p = fast_cast<Expression<GreaterThan>::VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::type e) {
    ptr<Expression<GreaterThanOrEqual>::VisitorBaseType>::type p = fast_cast<Expression<GreaterThanOrEqual>::VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::type e) {
    ptr<Expression<Reference<Variable> >::VisitorBaseType>::type p = fast_cast<Expression<Reference<Variable> >::VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference<Function> > >::type e) {
    ptr<Expression<Reference<Function> >::VisitorBaseType>::type p = fast_cast<Expression<Reference<Function> >::VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference<Constant<Base> > > >::type e) {
    ptr<Expression<Reference<Constant<Base> > >::VisitorBaseType>::type p = fast_cast<Expression<Reference<Constant<Base> > >::VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Load> >::type e) {
    ptr<Expression<Load>::VisitorBaseType>::type p =
      fast_cast<Expression<Load>::VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<TuplePointer> >::type e) {
    ptr<Expression<TuplePointer>::VisitorBaseType>::type p =
      fast_cast<Expression<TuplePointer>::VisitorBaseType>(e);
    visit(p);
  }
}
