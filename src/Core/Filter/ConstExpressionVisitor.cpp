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
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Base> > e) {}
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const InnerExpression> e) {
    typedef detail::VisitorBaseTypeOf<InnerExpression>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const InnerExpressionBase> e) {
    typedef detail::VisitorBaseTypeOf<InnerExpressionBase>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(static_cast<ptr<const VisitorBaseType>>(e));
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const LeafExpression> e) {
    typedef detail::VisitorBaseTypeOf<LeafExpression>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  // Establish an order for properties.  This coresponds to the
  // property tag ordering.
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Arithmetic> > e) {
    if (ptr<const Expression<Unary> > ue =
	dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<const Expression<Binary> > be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Arithmetic>::VisitorBaseType VisitorBaseType;
      ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Logical> > e) {
    if (ptr<const Expression<Unary> > ue =
	dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<const Expression<Binary> > be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Logical>::VisitorBaseType VisitorBaseType;
      ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Bitwise> > e) {
    if (ptr<const Expression<Unary> > ue =
	dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<const Expression<Binary> > be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Bitwise>::VisitorBaseType VisitorBaseType;
      ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Relational> > e) {
    if (ptr<const Expression<Unary> > ue =
	dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<const Expression<Binary> > be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Relational>::VisitorBaseType VisitorBaseType;
      ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Commutative> > e) {
    if (ptr<const Expression<Bitwise> > be =
	dyn_cast<const Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<const Expression<Logical> > le =
	     dyn_cast<const Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<const Expression<Arithmetic> > ae =
	     dyn_cast<const Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<const Expression<Unary> > ue =
	     dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<const Expression<Binary> > be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Commutative>::VisitorBaseType VisitorBaseType;
      ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Associative> > e) {
    if (ptr<const Expression<Commutative> > ce =
	dyn_cast<const Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<const Expression<Bitwise> > be =
	     dyn_cast<const Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<const Expression<Logical> > le =
	     dyn_cast<const Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<const Expression<Arithmetic> > ae =
	     dyn_cast<const Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<const Expression<Unary> > ue =
	     dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<const Expression<Binary> > be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Associative>::VisitorBaseType VisitorBaseType;
      ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Transitive> > e) {
    if (ptr<const Expression<Associative> > ase =
	dyn_cast<const Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<const Expression<Commutative> > ce =
	     dyn_cast<const Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<const Expression<Bitwise> > be =
	     dyn_cast<const Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<const Expression<Logical> > le =
	     dyn_cast<const Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<const Expression<Arithmetic> > ae =
	     dyn_cast<const Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<const Expression<Unary> > ue =
	     dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<const Expression<Binary> > be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Transitive>::VisitorBaseType VisitorBaseType;
      ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
      visit(p);
    }
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Reflexive> > e) {
    if (ptr<const Expression<Transitive> > te =
	dyn_cast<const Expression<Transitive> >(e)) {
      visit(te);
    }
    else if (ptr<const Expression<Associative> > ase =
	dyn_cast<const Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<const Expression<Commutative> > ce =
	     dyn_cast<const Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<const Expression<Bitwise> > be =
	     dyn_cast<const Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<const Expression<Logical> > le =
	     dyn_cast<const Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<const Expression<Arithmetic> > ae =
	     dyn_cast<const Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<const Expression<Unary> > ue =
	     dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<const Expression<Binary> > be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Reflexive>::VisitorBaseType VisitorBaseType;
      ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
      visit(p);
    }
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Ref> > e) {
    if (ptr<const Expression<Reflexive> > re =
	dyn_cast<const Expression<Reflexive> >(e)) {
      visit(re);
    }
    else if (ptr<const Expression<Transitive> > te =
	dyn_cast<const Expression<Transitive> >(e)) {
      visit(te);
    }
    else if (ptr<const Expression<Associative> > ase =
	dyn_cast<const Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<const Expression<Commutative> > ce =
	     dyn_cast<const Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<const Expression<Bitwise> > be =
	     dyn_cast<const Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<const Expression<Logical> > le =
	     dyn_cast<const Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<const Expression<Arithmetic> > ae =
	     dyn_cast<const Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<const Expression<Unary> > ue =
	     dyn_cast<const Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<const Expression<Binary> > be =
	     dyn_cast<const Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Ref>::VisitorBaseType VisitorBaseType;
      ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
      visit(p);
    }
  }

ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Unary> > e) {
  typedef detail::VisitorBaseTypeOfExpression<Unary>::VisitorBaseType VisitorBaseType;
  ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
  visit(p);
}
ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Binary> > e) {
  typedef detail::VisitorBaseTypeOfExpression<Binary>::VisitorBaseType VisitorBaseType;
  ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
  visit(p);
}

  // We always prefer to visit the properties because they give us
  // semantic information.
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Add> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Add>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Subtract> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Subtract>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Divide> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Divide>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Multiply> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Multiply>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Modulus> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Modulus>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Negate> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Negate>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<LogicalAnd> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalAnd>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<LogicalOr> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalOr>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<LogicalNot> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalNot>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<BitwiseAnd> > e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseAnd>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<BitwiseOr> > e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseOr>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<BitwiseXor> > e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseXor>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<BitwiseComplement> > e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseComplement>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<ShiftLeft> > e) {
    typedef detail::VisitorBaseTypeOfExpression<ShiftLeft>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<ArithmeticShiftRight> > e) {
    typedef detail::VisitorBaseTypeOfExpression<ArithmeticShiftRight>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<LogicalShiftRight> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalShiftRight>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<LessThan> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LessThan>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<LessThanOrEqual> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LessThanOrEqual>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Equal> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Equal>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<NotEqual> > e) {
    typedef detail::VisitorBaseTypeOfExpression<NotEqual>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<GreaterThan> > e) {
    typedef detail::VisitorBaseTypeOfExpression<GreaterThan>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<GreaterThanOrEqual> > e) {
    typedef detail::VisitorBaseTypeOfExpression<GreaterThanOrEqual>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Reference<Variable> > > e) {
    typedef detail::VisitorBaseTypeOfExpression<Reference<Variable> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Reference<Function> > > e) {
    typedef detail::VisitorBaseTypeOfExpression<Reference<Function> >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Reference<Constant<Base> > > > e) {
    typedef detail::VisitorBaseTypeOfExpression<Reference<Constant<Base> > >::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<Load> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Load>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }

  ConstExpressionVisitor::result_type ConstExpressionVisitor::visit(ptr<const Expression<TuplePointer> > e) {
    typedef detail::VisitorBaseTypeOfExpression<TuplePointer>::VisitorBaseType VisitorBaseType;
    ptr<const VisitorBaseType> p = fast_cast<const VisitorBaseType>(e);
    visit(p);
  }
}
