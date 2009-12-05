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
    virtual result_type visit(Statement<Base> &);
    virtual result_type visit(LeafStatement &s);
    virtual result_type visit(InnerStatement &s);
    virtual result_type visit(Statement<Conditional> &s);
    virtual result_type visit(Statement<Iterative> &s);
    virtual result_type visit(Statement<Mutating> &s);
    virtual result_type visit(Statement<Block> &s);
    virtual result_type visit(Statement<IfThen> &s);
    virtual result_type visit(Statement<IfElse> &s);
    virtual result_type visit(Statement<While> &s);
    virtual result_type visit(Statement<DoWhile> &s);
    virtual result_type visit(Statement<Switch> &s);
    virtual result_type visit(Statement<Case> &s);
    virtual result_type visit(Statement<CaseBlock> &s);
    virtual result_type visit(Statement<Before> &s);
    virtual result_type visit(Statement<After> &s);
    virtual result_type visit(Statement<Goto> &s);
    virtual result_type visit(Statement<Return> &s);
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
    virtual result_type visit(Expression<Base> &);
    virtual result_type visit(InnerExpression &);
    virtual result_type visit(LeafExpression &);
    virtual result_type visit(Expression<Arithmetic> &);
    virtual result_type visit(Expression<Logical> &);
    virtual result_type visit(Expression<Bitwise> &);
    virtual result_type visit(Expression<Ref> &);
    virtual result_type visit(Expression<Commutative> &);
    virtual result_type visit(Expression<Associative> &);
    virtual result_type visit(Expression<Transitive> &);
    virtual result_type visit(Expression<Reflexive> &);
    virtual result_type visit(Expression<Add> &);
    virtual result_type visit(Expression<Subtract> &);
    virtual result_type visit(Expression<Divide> &);
    virtual result_type visit(Expression<Multiply> &);
    virtual result_type visit(Expression<Modulus> &);
    virtual result_type visit(Expression<Negate> &);
    virtual result_type visit(Expression<LogicalAnd> &);
    virtual result_type visit(Expression<LogicalOr> &);
    virtual result_type visit(Expression<LogicalNot> &);
    virtual result_type visit(Expression<BitwiseAnd> &);
    virtual result_type visit(Expression<BitwiseOr> &);
    virtual result_type visit(Expression<BitwiseComplement> &);
    virtual result_type visit(Expression<Reference<Variable> > &);
  };
}

#endif
