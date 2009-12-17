#include <mirv/core/filter/visitor.hh>
#include <mirv/core/util/cast.hh>

namespace mirv {
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Base> >::type s) {};
  StatementVisitor::result_type StatementVisitor::visit(ptr<LeafStatement>::type s) {
    visit(static_cast<ptr<Statement<Base> >::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<InnerStatement>::type s) {
    visit(static_cast<ptr<Statement<Base> >::type>(s));
  }
  // Establish an order for properties:
  // Mutating->Iterative->Conditional.  This coresponds to the
  // property tag ordering.
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Conditional> >::type s) {
    visit(static_cast<ptr<Statement<Base> >::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Iterative> >::type s) {
    if (ptr<Statement<Conditional> >::type cs = 
	dyn_cast<Statement<Conditional> >(s)) {
      visit(cs);
    }
    else {
      visit(static_cast<ptr<Statement<Base> >::type>(s));
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
      visit(static_cast<ptr<Statement<Base> >::type>(s));
    }
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Block> >::type s) {
    visit(static_cast<ptr<Statement<Block>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfThen> >::type s) {
    visit(static_cast<ptr<Statement<IfThen>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfElse> >::type s) {
    visit(static_cast<ptr<Statement<IfElse>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<While> >::type s) {
    visit(static_cast<ptr<Statement<While>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<DoWhile> >::type s) {
    visit(static_cast<ptr<Statement<DoWhile>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Switch> >::type s) {
    visit(static_cast<ptr<Statement<Switch>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Case> >::type s) {
    visit(static_cast<ptr<Statement<Case>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<CaseBlock> >::type s) {
    visit(static_cast<ptr<Statement<CaseBlock>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Before> >::type s) {
    visit(static_cast<ptr<Statement<Before>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<After> >::type s) {
    visit(static_cast<ptr<Statement<After>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Goto> >::type s) {
    visit(static_cast<ptr<Statement<Goto>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Return> >::type s) {
    visit(static_cast<ptr<Statement<After>::visitor_base_type>::type>(s));
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<BaseExpression>::type e) {}
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<InnerExpression>::type e) {
    visit(static_cast<ptr<Expression<Base> >::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<LeafExpression>::type e) {
    visit(static_cast<ptr<Expression<Base> >::type>(e));
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
      visit(static_cast<ptr<InnerExpression>::type>(e));
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
      visit(static_cast<ptr<InnerExpression>::type>(e));
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
      visit(static_cast<ptr<InnerExpression>::type>(e));
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
      visit(static_cast<ptr<InnerExpression>::type>(e));
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
      visit(static_cast<ptr<InnerExpression>::type>(e));
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
      visit(*ae);
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
      visit(static_cast<ptr<InnerExpression>::type>(e));
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
      visit(*ae);
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
      visit(static_cast<ptr<InnerExpression>::type>(e));
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference> >::type e) {
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
      visit(*ae);
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
      visit(static_cast<ptr<InnerExpression>::type>(e));
    }
  }
  // We always prefer to visit the properties because they give us
  // semantic information.
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Add> >::type e) {
    visit(static_cast<ptr<Expression<Add>::vistor_base_type>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Subtract> >::type e) {
    visit(static_cast<ptr<Expression<Subtract>::visitor_base_type>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Divide> >::type e) {
    visit(static_cast<ptr<Expression<Divide>::visitor_base_type>::type>>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Multiply> >::type e) {
    visit(static_cast<ptr<Expression<Multiply>::visitor_base_type>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Modulus> >::type e) {
    visit(static_cast<ptr<Expression<Modulus>::visitor_base_type>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Negate> >::type e) {
    visit(static_cast<ptr<Expression<Negate>::visitor_base_type>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::type e) {
    visit(static_cast<ptr<Expression<LogicalAnd>::visitor_base_type>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalOr> >::type e) {
    visit(static_cast<ptr<Expression<LogicalOr>::visitor_base_type>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalNot> >::type e) {
    visit(static_cast<ptr<Expression<LogicalNot>::visitor_base_type>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::type e) {
    visit(static_cast<ptr<Expression<BitwiseAnd>::visitor_base_type>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::type e) {
    visit(static_cast<ptr<Expression<BitwiseOr>::visitor_base_type>::type>(e));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::type e) {
    visit(static_cast<ptr<Expression<BitwiseComplement>::type>(e));
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::type e) {
    visit(static_cast<ptr<Expression<Reference<Variable> >::visitor_base_type>::type>(e));
  }
}
