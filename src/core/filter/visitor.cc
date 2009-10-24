#include <mirv/filter/visitor.hh>

namespace mirv {
  virtual result_type StatementVisitor::visit(BaseStatement &) {};
  virtual result_type StatementVisitor::visit(LeafStatement &s) {
    visit(static_cast<BaseStatement &>(s));
  }
  virtual result_type StatementVisitor::visit(InnerStatement &s) {
    visit(static_cast<BaseStatement &>(s));
  }
  // Establish an order for properties:
  // Mutating->Iterative->Conditional.  This coresponds to the
  // property tag ordering.
  virtual result_type StatementVisitor::visit(Statement<Conditional, void> &s) {
    visit(static_cast<BaseStatement &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<Iterative, void> &s) {
    if (Statement<Conditional, void> *cs = 
	dynamic_cast<Statement<Conditional, void> *>(&s)) {
      visit(*cs);
    }
    else {
      visit(static_cast<BaseStatement &>(s));
    }
  }
  virtual result_type StatementVisitor::visit(Statement<Mutating, void> &s) {
    if (Statement<Iterative, void> *is = 
	dynamic_cast<Statement<Iterative, void> *>(&s)) {
      visit(*is);
    }
    else if (Statement<Conditional, void> *cs = 
	     dynamic_cast<Statement<Conditional, void> *>(&s)) {
      visit(*cs);
    }
    else {
      visit(static_cast<BaseStatement &>(s));
    }
  }
  virtual result_type StatementVisitor::visit(Statement<Block> &s) {
    visit(static_cast<Statement<Block>::visitor_base_type &>(s));
  }
  // We always prefer to visit the properties because they give us
  // semantic information.
  virtual result_type StatementVisitor::visit(Statement<IfThen> &s) {
    visit(static_cast<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<IfThen>::properties> 
	  >, void> &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<IfThen> &s) {
    visit(static_cast<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<IfThen>::properties> 
	  >, void> &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<IfElse> &s) {
    visit(static_cast<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<IfElse>::properties> 
	  >, void> &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<While> &s) {
    visit(static_cast<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<While>::properties> 
	  >, void> &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<DoWhile> &s) {
    visit(static_cast<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<DoWhile>::properties> 
	  >, void> &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<Switch> &s) {
    visit(static_cast<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<Switch>::properties> 
	  >, void> &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<Case> &s) {
    visit(static_cast<Statement<Case>::visitor_base_type &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<CaseBlock> &s) {
    visit(static_cast<Statement<CaseBlock>::visitor_base_type &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<Before> &s) {
    visit(static_cast<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<Before>::properties> 
	  >, void> &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<After> &s) {
    visit(static_cast<Statement<After>::visitor_base_type &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<Goto> &s) {
    visit(static_cast<Statement<boost::mpl::deref<
	  boost::mpl::begin<Statement<Goto>::properties> 
	  >, void> &>(s));
  }
  virtual result_type StatementVisitor::visit(Statement<Return> &s) {
    visit(static_cast<Statement<After>::visitor_base_type &>(s));
  }

  virtual result_type ExpressionVisitor::visit(BaseExpression &e) {}
  virtual result_type ExpressionVisitor::visit(InnerExpression &e) {
    visit(static_cast<BaseExpression &>(e));
  }
  virtual result_type ExpressionVisitor::visit(LeafExpression &e) {
    visit(static_cast<BaseExpression &>(e));
  }
  // Establish an order for properties.  This coresponds to the
  // property tag ordering.
  virtual result_type ExpressionVisitor::visit(Expression<Arithmetic, void> &e) {
    if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  virtual result_type ExpressionVisitor::visit(Expression<Logical, void> &e) {
    if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  virtual result_type ExpressionVisitor::visit(Expression<Bitwise, void> &e) {
    if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  virtual result_type ExpressionVisitor::visit(Expression<Commutative, void> &e) {
    if (Expression<Bitwise, void> *be = dynamic_cast<Expression<Bitwise, void> *>(&e)) {
      visit(*be);
    }
    else if (Expression<Logical, void> *le = dynamic_cast<Expression<Logical, void> *>(&e)) {
      visit(*le);
    }
    else if (Expression<Arithmetic, void> *ae = dynamic_cast<Expression<Arithmetic, void> *>(&e)) {
      visit(*ae);
    }
    else if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  virtual result_type ExpressionVisitor::visit(Expression<NonCommutative, void> &e) {
    if (Expression<Bitwise, void> *be = dynamic_cast<Expression<Bitwise, void> *>(&e)) {
      visit(*be);
    }
    else if (Expression<Logical, void> *le = dynamic_cast<Expression<Logical, void> *>(&e)) {
      visit(*le);
    }
    else if (Expression<Arithmetic, void> *ae = dynamic_cast<Expression<Arithmetic, void> *>(&e)) {
      visit(*ae);
    }
    else if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  virtual result_type ExpressionVisitor::visit(Expression<Associative, void> &e) {
    if (Expression<Commutative, void> *ce = dynamic_cast<Expression<Commutative, void> *>(&e)) {
      visit(*ce);
    }
    else if (Expression<Bitwise, void> *be = dynamic_cast<Expression<Bitwise, void> *>(&e)) {
      visit(*be);
    }
    else if (Expression<Logical, void> *le = dynamic_cast<Expression<Logical, void> *>(&e)) {
      visit(*le);
    }
    else if (Expression<Arithmetic, void> *ae = dynamic_cast<Expression<Arithmetic, void> *>(&e)) {
      visit(*ae);
    }
    else if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  virtual result_type ExpressionVisitor::visit(Expression<NonAssociative, void> &e) {
    if (Expression<NonCommutative, void> *nce = dynamic_cast<Expression<NonCommutative, void> *>(&e)) {
      visit(*nce);
    }
    else if (Expression<Commutative, void> *ce = dynamic_cast<Expression<Commutative, void> *>(&e)) {
      visit(*ce);
    }
    else if (Expression<Bitwise, void> *be = dynamic_cast<Expression<Bitwise, void> *>(&e)) {
      visit(*be);
    }
    else if (Expression<Logical, void> *le = dynamic_cast<Expression<Logical, void> *>(&e)) {
      visit(*le);
    }
    else if (Expression<Arithmetic, void> *ae = dynamic_cast<Expression<Arithmetic, void> *>(&e)) {
      visit(*ae);
    }
    else if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  virtual result_type ExpressionVisitor::visit(Expression<Transitive, void> &e) {
    if (Expression<NonAssociative, void> *nase = dynamic_cast<Expression<NonAssociative, void> *>(&e)) {
      visit(*nase);
    }
    else if (Expression<Associative, void> *ase = dynamic_cast<Expression<Associative, void> *>(&e)) {
      visit(*ase);
    }
    else if (Expression<NonCommutative, void> *nce = dynamic_cast<Expression<NonCommutative, void> *>(&e)) {
      visit(*nce);
    }
    else if (Expression<Commutative, void> *ce = dynamic_cast<Expression<Commutative, void> *>(&e)) {
      visit(*ce);
    }
    else if (Expression<Bitwise, void> *be = dynamic_cast<Expression<Bitwise, void> *>(&e)) {
      visit(*be);
    }
    else if (Expression<Logical, void> *le = dynamic_cast<Expression<Logical, void> *>(&e)) {
      visit(*le);
    }
    else if (Expression<Arithmetic, void> *ae = dynamic_cast<Expression<Arithmetic, void> *>(&e)) {
      visit(*ae);
    }
    else if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  virtual result_type ExpressionVisitor::visit(Expression<Intransitive, void> &e) {
    if (Expression<NonAssociative, void> *nase = dynamic_cast<Expression<NonAssociative, void> *>(&e)) {
      visit(*nase);
    }
    else if (Expression<Associative, void> *ase = dynamic_cast<Expression<Associative, void> *>(&e)) {
      visit(*ase);
    }
    else if (Expression<NonCommutative, void> *nce = dynamic_cast<Expression<NonCommutative, void> *>(&e)) {
      visit(*nce);
    }
    else if (Expression<Commutative, void> *ce = dynamic_cast<Expression<Commutative, void> *>(&e)) {
      visit(*ce);
    }
    else if (Expression<Bitwise, void> *be = dynamic_cast<Expression<Bitwise, void> *>(&e)) {
      visit(*be);
    }
    else if (Expression<Logical, void> *le = dynamic_cast<Expression<Logical, void> *>(&e)) {
      visit(*le);
    }
    else if (Expression<Arithmetic, void> *ae = dynamic_cast<Expression<Arithmetic, void> *>(&e)) {
      visit(*ae);
    }
    else if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  virtual result_type ExpressionVisitor::visit(Expression<Reflexive, void> &e) {
    if (Expression<Intransitive, void> *ite = dynamic_cast<Expression<Intransitive, void> *>(&e)) {
      visit(*ite);
    }
    else if (Expression<Transitive, void> *te = dynamic_cast<Expression<Transitive, void> *>(&e)) {
      visit(*te);
    }
    else if (Expression<NonAssociative, void> *nase = dynamic_cast<Expression<NonAssociative, void> *>(&e)) {
      visit(*nase);
    }
    else if (Expression<Associative, void> *ase = dynamic_cast<Expression<Associative, void> *>(&e)) {
      visit(*ase);
    }
    else if (Expression<NonCommutative, void> *nce = dynamic_cast<Expression<NonCommutative, void> *>(&e)) {
      visit(*nce);
    }
    else if (Expression<Commutative, void> *ce = dynamic_cast<Expression<Commutative, void> *>(&e)) {
      visit(*ce);
    }
    else if (Expression<Bitwise, void> *be = dynamic_cast<Expression<Bitwise, void> *>(&e)) {
      visit(*be);
    }
    else if (Expression<Logical, void> *le = dynamic_cast<Expression<Logical, void> *>(&e)) {
      visit(*le);
    }
    else if (Expression<Arithmetic, void> *ae = dynamic_cast<Expression<Arithmetic, void> *>(&e)) {
      visit(*ae);
    }
    else if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  virtual result_type ExpressionVisitor::visit(Expression<NonReflexive, void> &e) {
    if (Expression<Intransitive, void> *ite = dynamic_cast<Expression<Intransitive, void> *>(&e)) {
      visit(*ite);
    }
    else if (Expression<Transitive, void> *te = dynamic_cast<Expression<Transitive, void> *>(&e)) {
      visit(*te);
    }
    else if (Expression<NonAssociative, void> *nase = dynamic_cast<Expression<NonAssociative, void> *>(&e)) {
      visit(*nase);
    }
    else if (Expression<Associative, void> *ase = dynamic_cast<Expression<Associative, void> *>(&e)) {
      visit(*ase);
    }
    else if (Expression<NonCommutative, void> *nce = dynamic_cast<Expression<NonCommutative, void> *>(&e)) {
      visit(*nce);
    }
    else if (Expression<Commutative, void> *ce = dynamic_cast<Expression<Commutative, void> *>(&e)) {
      visit(*ce);
    }
    else if (Expression<Bitwise, void> *be = dynamic_cast<Expression<Bitwise, void> *>(&e)) {
      visit(*be);
    }
    else if (Expression<Logical, void> *le = dynamic_cast<Expression<Logical, void> *>(&e)) {
      visit(*le);
    }
    else if (Expression<Arithmetic, void> *ae = dynamic_cast<Expression<Arithmetic, void> *>(&e)) {
      visit(*ae);
    }
    else if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  virtual result_type ExpressionVisitor::visit(Expression<Reference, void> &e) {
    if (Expression<NonReflexive, void> *nre = dynamic_cast<Expression<NonReflexive, void> *>(&e)) {
      visit(*nre);
    }
    else if (Expression<Reflexive, void> *re = dynamic_cast<Expression<Reflexive, void> *>(&e)) {
      visit(*re);
    }
    else if (Expression<Intransitive, void> *ite = dynamic_cast<Expression<Intransitive, void> *>(&e)) {
      visit(*ite);
    }
    else if (Expression<Transitive, void> *te = dynamic_cast<Expression<Transitive, void> *>(&e)) {
      visit(*te);
    }
    else if (Expression<NonAssociative, void> *nase = dynamic_cast<Expression<NonAssociative, void> *>(&e)) {
      visit(*nase);
    }
    else if (Expression<Associative, void> *ase = dynamic_cast<Expression<Associative, void> *>(&e)) {
      visit(*ase);
    }
    else if (Expression<NonCommutative, void> *nce = dynamic_cast<Expression<NonCommutative, void> *>(&e)) {
      visit(*nce);
    }
    else if (Expression<Commutative, void> *ce = dynamic_cast<Expression<Commutative, void> *>(&e)) {
      visit(*ce);
    }
    else if (Expression<Bitwise, void> *be = dynamic_cast<Expression<Bitwise, void> *>(&e)) {
      visit(*be);
    }
    else if (Expression<Logical, void> *le = dynamic_cast<Expression<Logical, void> *>(&e)) {
      visit(*le);
    }
    else if (Expression<Arithmetic, void> *ae = dynamic_cast<Expression<Arithmetic, void> *>(&e)) {
      visit(*ae);
    }
    else if (UnaryExpression *ue = dynamic_cast<UnaryExpression *>(&e)) {
      visit(*ue);
    }
    else if (BinaryExpression be = dynamic_cast<BinaryExpression *>(&e)) {
      visit(*be);
    }
    else {
      visit(static_cast<InnerExpression &>(e));
    }
  }
  // We always prefer to visit the properties because they give us
  // semantic information.
  virtual result_type ExpressionVisitor::visit(Expression<Add> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Add>::properties> 
	  >, void> &>(s));
  }
  virtual result_type ExpressionVisitor::visit(Expression<Subtract> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Subtract>::properties> 
	  >, void> &>(s));
  }
  virtual result_type ExpressionVisitor::visit(Expression<Divide> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Divide>::properties> 
	  >, void> &>(s));
  }
  virtual result_type ExpressionVisitor::visit(Expression<Multiply> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Multiply>::properties> 
	  >, void> &>(s));
  }
  virtual result_type ExpressionVisitor::visit(Expression<Modulus> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Modulus>::properties> 
	  >, void> &>(s));
  }
  virtual result_type ExpressionVisitor::visit(Expression<Negate> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<Negate>::properties> 
	  >, void> &>(s));
  }
  virtual result_type ExpressionVisitor::visit(Expression<LogicalAnd> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<LogicalAnd>::properties> 
	  >, void> &>(s));
  }
  virtual result_type ExpressionVisitor::visit(Expression<LogicalOr> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<LogicalOr>::properties> 
	  >, void> &>(s));
  }
  virtual result_type ExpressionVisitor::visit(Expression<LogicalNot> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<LogicalNot>::properties> 
	  >, void> &>(s));
  }
  virtual result_type ExpressionVisitor::visit(Expression<BitwiseAnd> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<BitwiseAnd>::properties> 
	  >, void> &>(s));
  }
  virtual result_type ExpressionVisitor::visit(Expression<BitwiseOr> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<BitwiseOr>::properties> 
	  >, void> &>(s));
  }
  virtual result_type ExpressionVisitor::visit(Expression<BitwiseComplement> &e) {
    visit(static_cast<Expression<boost::mpl::deref<
	  boost::mpl::begin<Expression<BitwiseComplement>::properties> 
	  >, void> &>(s));
  }
}
