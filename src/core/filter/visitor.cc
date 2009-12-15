#include <mirv/core/filter/visitor.hh>

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
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Conditional, void> >::type s) {
    visit(static_cast<ptr<Statement<Base> >::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Iterative, void> >::type s) {
    if (ptr<Statement<Conditional> >::type cs = 
	dyn_cast<ptr<Statement<Conditional> >::type>(s)) {
      visit(cs);
    }
    else {
      visit(static_cast<ptr<Statement<Base> >::type>(s));
    }
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Mutating, void> >::type s) {
    if (ptr<Statement<Iterative> >::type is = 
	dyn_cast<ptr<Statement<Iterative> >::type>(s)) {
      visit(is);
    }
    else if (ptr<Statement<Conditional> >::type cs = 
	     dyn_cast<ptr<Statement<Conditional> >::type>(s)) {
      visit(cs);
    }
    else {
      visit(static_cast<ptr<Statement<Base> >::type>(s));
    }
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Block> >::type s) {
    visit(static_cast<ptr<Statement<Block>::visitor_base_type>::type>(s));
  }
  // We always prefer to visit the properties because they give us
  // semantic information.
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfThen> >::type s) {
    visit(static_cast<ptr<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<IfThen>::properties> >::type> >::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfThen> >::type s) {
    visit(static_cast<ptr<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<IfThen>::properties> 
	  > >::type > >::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<IfElse> >::type s) {
    visit(static_cast<ptr<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<IfElse>::properties> 
	  > >::type> >::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<While> >::type s) {
    visit(static_cast<ptr<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<While>::properties> 
	  >::type> >::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<DoWhile> >::type s) {
    visit(static_cast<ptr<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<DoWhile>::properties> 
	  >::type> >::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Switch> >::type s) {
    visit(static_cast<ptr<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<Switch>::properties> 
	  >::type> >::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Case> >::type s) {
    visit(static_cast<ptr<Statement<Case>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<CaseBlock> >::type s) {
    visit(static_cast<ptr<Statement<CaseBlock>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Before> >::type s) {
    visit(static_cast<ptr<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<Before>::properties> 
	  >::type> >::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<After> >::type s) {
    visit(static_cast<ptr<Statement<After>::visitor_base_type>::type>(s));
  }
  StatementVisitor::result_type StatementVisitor::visit(ptr<Statement<Goto> >::type s) {
    visit(static_cast<ptr<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<Goto>::properties> 
	  >::type> >::type>(s));
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
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Arithmetic, void> >::type e) {
    if (ptr<Expression<Unary> >::type ue =
	dyn_cast<ptr<Expression<Unary> >::type>(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<ptr<Expression<Binary> >::type>(e)) {
      visit(be);
    }
    else {
      visit(static_cast<ptr<InnerExpression>::type>(e));
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Logical, void> >::type e) {
    if (ptr<Expression<Unary> >::type ue =
	dyn_cast<ptr<Expression<Unary> >::type>(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<ptr<Expression<Binary> >::type>(e)) {
      visit(be);
    }
    else {
      visit(static_cast<ptr<InnerExpression>::type>(e));
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Bitwise, void> >::type e) {
    if (ptr<Expression<Unary> >::type ue =
	dyn_cast<ptr<Expression<Unary> >::type>(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<ptr<Expression<Binary> >::type>(e)) {
      visit(be);
    }
    else {
      visit(static_cast<ptr<InnerExpression>::type>(e));
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Commutative, void> >::type e) {
    if (ptr<Expression<Bitwise> >::type be =
	dyn_cast<ptr<Expression<Bitwise> >::type>(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::type le =
	     dyn_cast<ptr<Expression<Logical> >::type>(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::type ae =
	     dyn_cast<ptr<Expression<Arithmetic> >::type>(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::type ue =
	     dyn_cast<ptr<Expression<Unary> >::type>(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<ptr<Expression<Binary> >::type>(e)) {
      visit(be);
    }
    else {
      visit(static_cast<ptr<InnerExpression>::type>(e));
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Associative, void> >::type e) {
    if (ptr<Expression<Commutative> >::type ce =
	dyn_cast<ptr<Expression<Commutative> >::type>(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::type be =
	     dyn_cast<ptr<Expression<Bitwise> >::type>(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::type le =
	     dyn_cast<ptr<Expression<Logical> >::type>(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::type ae =
	     dyn_cast<ptr<Expression<Arithmetic> >::type>(e)) {
      visit(ae);
    }
    else if (ptr<Expression<Unary> >::type ue =
	     dyn_cast<ptr<Expression<Unary> >::type>(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<ptr<Expression<Binary> >::type>(e)) {
      visit(be);
    }
    else {
      visit(static_cast<ptr<InnerExpression>::type>(e));
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Transitive> >::type e) {
    if (ptr<Expression<Associative> >::type ase =
	dyn_cast<ptr<Expression<Associative> >::type>(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> >::type ce =
	     dyn_cast<ptr<Expression<Commutative> >::type>(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::type be =
	     dyn_cast<ptr<Expression<Bitwise> >::type>(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::type le =
	     dyn_cast<ptr<Expression<Logical> >::type>(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::type ae =
	     dyn_cast<ptr<Expression<Arithmetic> >::type>(e)) {
      visit(*ae);
    }
    else if (ptr<Expression<Unary> >::type ue =
	     dyn_cast<ptr<Expression<Unary> >::type>(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<ptr<Expression<Binary> >::type>(e)) {
      visit(be);
    }
    else {
      visit(static_cast<ptr<InnerExpression>::type>(e));
    }
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reflexive, void> >::type e) {
    if (ptr<Expression<Transitive> >::type te =
	dyn_cast<ptr<Expression<Transitive> >::type>(e)) {
      visit(te);
    }
    else if (ptr<Expression<Associative> >::type ase =
	dyn_cast<ptr<Expression<Associative> >::type>(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> >::type ce =
	     dyn_cast<ptr<Expression<Commutative> >::type>(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::type be =
	     dyn_cast<ptr<Expression<Bitwise> >::type>(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::type le =
	     dyn_cast<ptr<Expression<Logical> >::type>(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::type ae =
	     dyn_cast<ptr<Expression<Arithmetic> >::type>(e)) {
      visit(*ae);
    }
    else if (ptr<Expression<Unary> >::type ue =
	     dyn_cast<ptr<Expression<Unary> >::type>(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<ptr<Expression<Binary> >::type>(e)) {
      visit(be);
    }
    else {
      visit(static_cast<ptr<InnerExpression>::type>(e));
    }
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference, void> >::type e) {
    if (ptr<Expression<Reflexive> >::type re =
	dyn_cast<ptr<Expression<Reflexive> >::type>(e)) {
      visit(re);
    }
    else if (ptr<Expression<Transitive> >::type te =
	dyn_cast<ptr<Expression<Transitive> >::type>(e)) {
      visit(te);
    }
    else if (ptr<Expression<Associative> >::type ase =
	dyn_cast<ptr<Expression<Associative> >::type>(e)) {
      visit(ase);
    }
    else if (ptr<Expression<Commutative> >::type ce =
	     dyn_cast<ptr<Expression<Commutative> >::type>(e)) {
      visit(ce);
    }
    else if (ptr<Expression<Bitwise> >::type be =
	     dyn_cast<ptr<Expression<Bitwise> >::type>(e)) {
      visit(be);
    }
    else if (ptr<Expression<Logical> >::type le =
	     dyn_cast<ptr<Expression<Logical> >::type>(e)) {
      visit(le);
    }
    else if (ptr<Expression<Arithmetic> >::type ae =
	     dyn_cast<ptr<Expression<Arithmetic> >::type>(e)) {
      visit(*ae);
    }
    else if (ptr<Expression<Unary> >::type ue =
	     dyn_cast<ptr<Expression<Unary> >::type>(e)) {
      visit(ue);
    }
    else if (ptr<Expression<Binary> >::type be =
	     dyn_cast<ptr<Expression<Binary> >::type>(e)) {
      visit(be);
    }
    else {
      visit(static_cast<ptr<InnerExpression>::type>(e));
    }
  }
  // We always prefer to visit the properties because they give us
  // semantic information.
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Add> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Add>::properties> 
	  >::type> >::type>(s));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Subtract> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Subtract>::properties> 
	  >::type> >::type>(s));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Divide> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Divide>::properties> 
	  >::type> >::type>(s));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Multiply> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Multiply>::properties> 
	  >::type> >::type>(s));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Modulus> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Modulus>::properties> 
	  >::type> >::type>(s));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Negate> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Negate>::properties> 
	  >::type> > >::type>(s));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<LogicalAnd>::properties> 
	  >::type> >::type>(s));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalOr> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<LogicalOr>::properties> 
	  >::type> >::type>(s));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<LogicalNot> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<LogicalNot>::properties> 
	  >::type> >::type>(s));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<BitwiseAnd>::properties> 
	  >::type> >::type>(s));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<BitwiseOr>::properties> 
	  >::type> >::type>(s));
  }
  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<BitwiseComplement>::properties> 
	  >::type> >::type>(s));
  }

  ExpressionVisitor::result_type ExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::type e) {
    visit(static_cast<ptr<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Reference<Variable> >::properties> 
	  >::type> >::type>(s));
  }
}
