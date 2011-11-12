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
    typedef detail::VisitorBaseTypeOf<InnerExpression>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<InnerExpressionBase>::const_type e) {
    typedef detail::VisitorBaseTypeOf<InnerExpressionBase>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(static_cast<ptr<VisitorBaseType>::const_type>(e));
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<LeafExpression>::const_type e) {
    typedef detail::VisitorBaseTypeOf<LeafExpression>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
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
      typedef detail::VisitorBaseTypeOfExpression<Arithmetic>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
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
      typedef detail::VisitorBaseTypeOfExpression<Logical>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
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
      typedef detail::VisitorBaseTypeOfExpression<Bitwise>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
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
      typedef detail::VisitorBaseTypeOfExpression<Relational>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
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
      typedef detail::VisitorBaseTypeOfExpression<Commutative>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
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
      typedef detail::VisitorBaseTypeOfExpression<Associative>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
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
      typedef detail::VisitorBaseTypeOfExpression<Transitive>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
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
      typedef detail::VisitorBaseTypeOfExpression<Reflexive>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
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
      typedef detail::VisitorBaseTypeOfExpression<Ref>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
      visit(p);
    }
  }

ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Unary> >::const_type e) {
  typedef detail::VisitorBaseTypeOfExpression<Unary>::VisitorBaseType VisitorBaseType;
  ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
  visit(p);
}
ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Binary> >::const_type e) {
  typedef detail::VisitorBaseTypeOfExpression<Binary>::VisitorBaseType VisitorBaseType;
  ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
  visit(p);
}

  // We always prefer to visit the properties because they give us
  // semantic information.
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Add> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<Add>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Subtract> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<Subtract>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Divide> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<Divide>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Multiply> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<Multiply>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Modulus> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<Modulus>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Negate> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<Negate>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalAnd>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LogicalOr> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalOr>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LogicalNot> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalNot>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseAnd>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseOr>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<BitwiseXor> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseXor>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseComplement>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<ShiftLeft> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<ShiftLeft>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<ArithmeticShiftRight> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<ArithmeticShiftRight>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LogicalShiftRight> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalShiftRight>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LessThan> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<LessThan>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<LessThanOrEqual>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Equal> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<Equal>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<NotEqual> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<NotEqual>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<GreaterThan> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<GreaterThan>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<GreaterThanOrEqual>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<Reference<Variable> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Reference<Function> > >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<Reference<Function> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Reference<Constant<Base> > > >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<Reference<Constant<Base> > >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<Load> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<Load>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<Expression<TuplePointer> >::const_type e) {
    typedef detail::VisitorBaseTypeOfExpression<TuplePointer>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType>::const_type p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
}
