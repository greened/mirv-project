#include <mirv/core/filter/visitor.hh>
#include <mirv/core/util/cast.hh>

#include <mirv/core/ir/statement.hh>
#include <mirv/core/ir/control.hh>
#include <mirv/core/ir/mutating.hh>

#include <mirv/core/ir/expression.hh>
#include <mirv/core/ir/arithmetic.hh>
#include <mirv/core/ir/logical.hh>
#include <mirv/core/ir/bitwise.hh>
#include <mirv/core/ir/reference.hh>

namespace mirv {
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Base> >::type s) {};
  StatementVisitor::result_type StatementVisitor::visit(ptr<LeafStatement>::type s) {
    ptr<Statement<Base> >::type p = fast_cast<Statement<Base> >(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<InnerStatement>::type s) {
    ptr<Statement<Base> >::type p = fast_cast<Statement<Base> >(s);
    visit(p);
  }
  // Establish an order for properties:
  // Mutating->Iterative->Conditional.  This coresponds to the
  // property tag ordering.
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Conditional> >::type s) {
    ptr<Statement<Base> >::type p = fast_cast<Statement<Base> >(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Iterative> >::type s) {
    if (ptr<Statement<Conditional> >::type cs = 
	dyn_cast<Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      ptr<Statement<Base> >::type p = fast_cast<Statement<Base> >(s);
      visit(p);
    }
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Mutating> >::type s) {
    if (ptr<Statement<Iterative> >::type is = 
	dyn_cast<Statement<Iterative> >(s)) {
      visit(is);
    }
    else if (ptr<Statement<Conditional> >::type cs = 
	     dyn_cast<Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      ptr<Statement<Base> >::type p = fast_cast<Statement<Base> >(s);
      visit(p);
    }
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Block> >::type s) {
    ptr<Statement<Block>::visitor_base_type>::type p = fast_cast<Statement<Block>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<SingleBlock> >::type s) {
    ptr<Statement<SingleBlock>::visitor_base_type>::type p = fast_cast<Statement<SingleBlock>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<DualBlock> >::type s) {
    ptr<Statement<DualBlock>::visitor_base_type>::type p = fast_cast<Statement<DualBlock>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfThen> >::type s) {
    ptr<Statement<IfThen>::visitor_base_type>::type p = fast_cast<Statement<IfThen>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfElse> >::type s) {
    ptr<Statement<IfElse>::visitor_base_type>::type p = fast_cast<Statement<IfElse>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<While> >::type s) {
    ptr<Statement<While>::visitor_base_type>::type p = fast_cast<Statement<While>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<DoWhile> >::type s) {
    ptr<Statement<DoWhile>::visitor_base_type>::type p = fast_cast<Statement<DoWhile>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Switch> >::type s) {
    ptr<Statement<Switch>::visitor_base_type>::type p = fast_cast<Statement<Switch>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Case> >::type s) {
    ptr<Statement<Case>::visitor_base_type>::type p = fast_cast<Statement<Case>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<CaseBlock> >::type s) {
    ptr<Statement<CaseBlock>::visitor_base_type>::type p = fast_cast<Statement<CaseBlock>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Before> >::type s) {
    ptr<Statement<Before>::visitor_base_type>::type p = fast_cast<Statement<Before>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<After> >::type s) {
    ptr<Statement<After>::visitor_base_type>::type p = fast_cast<Statement<After>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Goto> >::type s) {
    ptr<Statement<Goto>::visitor_base_type>::type p = fast_cast<Statement<Goto>::visitor_base_type>(s);
    visit(p);
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Return> >::type s) {
    ptr<Statement<Return>::visitor_base_type>::type p = fast_cast<Statement<Return>::visitor_base_type>(s);
    visit(p);
  }

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

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::type e) {
    ptr<Expression<Reference<Variable> >::visitor_base_type>::type p = fast_cast<Expression<Reference<Variable> >::visitor_base_type>(e);
    visit(p);
  }
}
