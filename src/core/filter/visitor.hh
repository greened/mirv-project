#ifndef mirv_core_filter_visitor_hh
#define mirv_core_filter_visitor_hh

#include <visitor.hh>
 
#include <mirv/core/ir/control.hh>

#include <mirv/core/ir/arithmetic.hh>
#include <mirv/core/ir/logical.hh>
#include <mirv/core/ir/bitwise.hh>
#include <mirv/core/ir/reference.hh>

#include <mirv/core/ir/variable.hh>

#include <boost/mpl/vector.hpp>

namespace mirv {
  struct StatementVisitor :
    public lib::cyclic_visitor<void,
      boost::mpl::vector<
	Statement<Base>,
      LeafStatement,
      InnerStatement,
	Statement<Conditional>,
	Statement<Iterative>,
	Statement<Mutating>,
      Statement<Block>,
      Statement<IfThen>,
      Statement<IfElse>,
      Statement<While>,
      Statement<DoWhile>,
      Statement<Switch>,
      Statement<Case>,
      Statement<CaseBlock>,
      Statement<Before>,
      Statement<After>,
      Statement<Goto>,
      Statement<Return>
	> > {
  public:
    virtual result_type visit(ptr<Statement<Base> >::type);
    virtual result_type visit(ptr<LeafStatement>::type);
    virtual result_type visit(ptr<InnerStatement>::type);
    virtual result_type visit(ptr<Statement<Conditional> >::type);
    virtual result_type visit(ptr<Statement<Iterative> >::type);
    virtual result_type visit(ptr<Statement<Mutating> >::type);
    virtual result_type visit(ptr<Statement<Block> >::type);
    virtual result_type visit(ptr<Statement<IfThen> >::type);
    virtual result_type visit(ptr<Statement<IfElse> >::type);
    virtual result_type visit(ptr<Statement<While> >::type);
    virtual result_type visit(ptr<Statement<DoWhile> >::type);
    virtual result_type visit(ptr<Statement<Switch> >::type);
    virtual result_type visit(ptr<Statement<Case> >::type);
    virtual result_type visit(ptr<Statement<CaseBlock> >::type);
    virtual result_type visit(ptr<Statement<Before> >::type);
    virtual result_type visit(ptr<Statement<After> >::type);
    virtual result_type visit(ptr<Statement<Goto> >::type);
    virtual result_type visit(ptr<Statement<Return> >::type);
  };

  struct ExpressionVisitor :
    public lib::cyclic_visitor<void,
      boost::mpl::vector<
	Expression<Base>,
      InnerExpression,
      LeafExpression,
	Expression<Arithmetic>,
	Expression<Logical>,
	Expression<Bitwise>,
	Expression<Ref>,
	Expression<Commutative>,
	Expression<Associative>,
	Expression<Transitive>,
	Expression<Reflexive>,
      Expression<Add>,
      Expression<Subtract>,
      Expression<Divide>,
      Expression<Modulus>,
      Expression<Multiply>,
      Expression<Negate>,
      Expression<LogicalAnd>,

      Expression<LogicalOr>,
      Expression<LogicalNot>,
      Expression<BitwiseAnd>,
      Expression<BitwiseOr>,
	Expression<BitwiseComplement>,

	Expression<Reference<Variable> >
	> > {
  public:
    virtual result_type visit(ptr<Expression<Base> >::type);
    virtual result_type visit(ptr<InnerExpression>::type);
    virtual result_type visit(ptr<LeafExpression>::type);
    virtual result_type visit(ptr<Expression<Arithmetic> >::type);
    virtual result_type visit(ptr<Expression<Logical> >::type);
    virtual result_type visit(ptr<Expression<Bitwise> >::type);
    virtual result_type visit(ptr<Expression<Ref> >::type);
    virtual result_type visit(ptr<Expression<Commutative> >::type);
    virtual result_type visit(ptr<Expression<Associative> >::type);
    virtual result_type visit(ptr<Expression<Transitive> >::type);
    virtual result_type visit(ptr<Expression<Reflexive> >::type);
    virtual result_type visit(ptr<Expression<Add> >::type);
    virtual result_type visit(ptr<Expression<Subtract> >::type);
    virtual result_type visit(ptr<Expression<Divide> >::type);
    virtual result_type visit(ptr<Expression<Multiply> >::type);
    virtual result_type visit(ptr<Expression<Modulus> >::type);
    virtual result_type visit(ptr<Expression<Negate> >::type);
    virtual result_type visit(ptr<Expression<LogicalAnd> >::type);
    virtual result_type visit(ptr<Expression<LogicalOr> >::type);
    virtual result_type visit(ptr<Expression<LogicalNot> >::type);
    virtual result_type visit(ptr<Expression<BitwiseAnd> >::type);
    virtual result_type visit(ptr<Expression<BitwiseOr> >::type);
    virtual result_type visit(ptr<Expression<BitwiseComplement> >::type);
    virtual result_type visit(ptr<Expression<Reference<Variable> > >::type);
  };
}

#endif
