#ifndef mirv_core_filter_visitor_hh
#define mirv_core_filter_visitor_hh

#include <visitor.hh>
 
#include <mirv/ir/control.hh>

#include <mirv/ir/arithmetic.hh>
#include <mirv/ir/logical.hh>
#include <mirv/ir/bitwise.hh>

#include <boost/mpl/vector.hpp>

namespace mirv {
  struct StatementVisitor :
    public lib::cyclic_visitor<void,
      boost::mpl::vector<
      BaseStatement,
      LeafStatement,
      InnerStatement,
	Statement<Conditional, void>,
	Statement<Iterative, void>,
	Statement<Mutating, void>,
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
    virtual result_type visit(BaseStatement &);
    virtual result_type visit(LeafStatement &s);
    virtual result_type visit(InnerStatement &s);
    virtual result_type visit(Statement<Conditional, void> &s);
    virtual result_type visit(Statement<Iterative, void> &s);
    virtual result_type visit(Statement<Mutating, void> &s);
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
      BaseExpression,
      InnerExpression,
      LeafExpression,
	Expression<Arithmetic, void>,
	Expression<Logical, void>,
	Expression<Bitwise, void>,
	Expression<Commutative, void>,
	Expression<NonCommutative, void>,
	Expression<Associative, void>,
	Expression<NonAssociative, void>,
	Expression<Transitive, void>,
	Expression<Intransitive, void>,
	Expression<Reflexive, void>,
	Expression<NonReflexive, void>,
	Expression<Reference, void>,
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
      Expression<BitwiseComplement>
	> > {
  public:
    virtual result_type visit(BaseExpression &);
    virtual result_type visit(InnerExpression &);
    virtual result_type visit(LeafExpression &);
    virtual result_type visit(Expression<Arithmetic, void> &);
    virtual result_type visit(Expression<Logical, void> &);
    virtual result_type visit(Expression<Bitwise, void> &);
    virtual result_type visit(Expression<Commutative, void> &);
    virtual result_type visit(Expression<NonCommutative, void> &);
    virtual result_type visit(Expression<Associative, void> &);
    virtual result_type visit(Expression<NonAssociative, void> &);
    virtual result_type visit(Expression<Transitive, void> &);
    virtual result_type visit(Expression<Intransitive, void> &);
    virtual result_type visit(Expression<Reflexive, void> &);
    virtual result_type visit(Expression<NonReflexive, void> &);
    virtual result_type visit(Expression<Reference, void> &);
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
  };
}

#endif
