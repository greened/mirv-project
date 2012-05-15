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
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Base> > e) {}
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<InnerExpression> e) {
    typedef detail::VisitorBaseTypeOf<InnerExpression>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<InnerExpressionBase> e) {
    typedef detail::VisitorBaseTypeOf<InnerExpressionBase>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<LeafExpression> e) {
    typedef detail::VisitorBaseTypeOf<LeafExpression>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  // Establish an order for properties.  This coresponds to the
  // property tag ordering.
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Arithmetic> > e) {
    if (ptr<Expression<Unary> > ue =
	dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> > be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Arithmetic>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Logical> > e) {
    if (ptr<Expression<Unary> > ue =
	dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> > be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Logical>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Bitwise> > e) {
    if (ptr<Expression<Unary> > ue =
	dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> > be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Bitwise>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Relational> > e) {
    if (ptr<Expression<Unary> > ue =
	dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> > be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Relational>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Commutative> > e) {
    if (ptr<Expression<Bitwise> > be =
	dyn_cast<Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> > le =
	     dyn_cast<Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> > ae =
	     dyn_cast<Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> > ue =
	     dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> > be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Commutative>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Associative> > e) {
    if (ptr<Expression<Commutative> > ce =
	dyn_cast<Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> > be =
	     dyn_cast<Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> > le =
	     dyn_cast<Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> > ae =
	     dyn_cast<Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> > ue =
	     dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> > be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Associative>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Transitive> > e) {
    if (ptr<Expression<Associative> > ase =
	dyn_cast<Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> > ce =
	     dyn_cast<Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> > be =
	     dyn_cast<Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> > le =
	     dyn_cast<Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> > ae =
	     dyn_cast<Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> > ue =
	     dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> > be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Transitive>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
      visit(p);
    }
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reflexive> > e) {
    if (ptr<Expression<Transitive> > te =
	dyn_cast<Expression<Transitive> >(e)) {
      visit(te);
    }
    else if (ptr<Expression<Associative> > ase =
	dyn_cast<Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> > ce =
	     dyn_cast<Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> > be =
	     dyn_cast<Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> > le =
	     dyn_cast<Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> > ae =
	     dyn_cast<Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> > ue =
	     dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> > be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Reflexive>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
      visit(p);
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Ref> > e) {
    if (ptr<Expression<Reflexive> > re =
	dyn_cast<Expression<Reflexive> >(e)) {
      visit(re);
    }
    else if (ptr<Expression<Transitive> > te =
	dyn_cast<Expression<Transitive> >(e)) {
      visit(te);
    }
    else if (ptr<Expression<Associative> > ase =
	dyn_cast<Expression<Associative> >(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> > ce =
	     dyn_cast<Expression<Commutative> >(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> > be =
	     dyn_cast<Expression<Bitwise> >(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> > le =
	     dyn_cast<Expression<Logical> >(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> > ae =
	     dyn_cast<Expression<Arithmetic> >(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> > ue =
	     dyn_cast<Expression<Unary> >(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> > be =
	     dyn_cast<Expression<Binary> >(e)) {
      visit(be);
    }
    else {
      typedef detail::VisitorBaseTypeOfExpression<Ref>::VisitorBaseType VisitorBaseType;
      ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
      visit(p);
    }
  }

ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Unary> > e) {
  typedef detail::VisitorBaseTypeOfExpression<Unary>::VisitorBaseType VisitorBaseType;
  ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
  visit(p);
}
ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Binary> > e) {
  typedef detail::VisitorBaseTypeOfExpression<Binary>::VisitorBaseType VisitorBaseType;
  ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
  visit(p);
}

  // We always prefer to visit the properties because they give us
  // semantic information.
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Add> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Add>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Subtract> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Subtract>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Divide> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Divide>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Multiply> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Multiply>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Modulus> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Modulus>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Negate> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Negate>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalAnd> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalAnd>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalOr> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalOr>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalNot> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalNot>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseAnd> > e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseAnd>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseOr> > e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseOr>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseXor> > e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseXor>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseComplement> > e) {
    typedef detail::VisitorBaseTypeOfExpression<BitwiseComplement>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<ShiftLeft> > e) {
    typedef detail::VisitorBaseTypeOfExpression<ShiftLeft>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<ArithmeticShiftRight> > e) {
    typedef detail::VisitorBaseTypeOfExpression<ArithmeticShiftRight>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalShiftRight> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LogicalShiftRight>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LessThan> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LessThan>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> > e) {
    typedef detail::VisitorBaseTypeOfExpression<LessThanOrEqual>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Equal> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Equal>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<NotEqual> > e) {
    typedef detail::VisitorBaseTypeOfExpression<NotEqual>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<GreaterThan> > e) {
    typedef detail::VisitorBaseTypeOfExpression<GreaterThan>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> > e) {
    typedef detail::VisitorBaseTypeOfExpression<GreaterThanOrEqual>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference<Variable> > > e) {
    typedef detail::VisitorBaseTypeOfExpression<Reference<Variable> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference<Function> > > e) {
    typedef detail::VisitorBaseTypeOfExpression<Reference<Function> >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference<Constant<Base> > > > e) {
    typedef detail::VisitorBaseTypeOfExpression<Reference<Constant<Base> > >::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Load> > e) {
    typedef detail::VisitorBaseTypeOfExpression<Load>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<TuplePointer> > e) {
    typedef detail::VisitorBaseTypeOfExpression<TuplePointer>::VisitorBaseType VisitorBaseType;
    ptr<VisitorBaseType> p = fast_cast<VisitorBaseType>(e);
    visit(p);
  }
}
