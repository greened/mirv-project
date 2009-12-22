#ifndef mirv_core_filter_visitor_hh
#define mirv_core_filter_visitor_hh

#include <visitor.hh>
 
#include <mirv/core/ir/statement_fwd.hh>
#include <mirv/core/ir/control_fwd.hh>

#include <mirv/core/ir/expression_fwd.hh>
#include <mirv/core/ir/arithmetic_fwd.hh>
#include <mirv/core/ir/logical_fwd.hh>
#include <mirv/core/ir/bitwise_fwd.hh>
#include <mirv/core/ir/reference_fwd.hh>

#include <mirv/core/ir/variable_fwd.hh>

#include <boost/mpl/vector.hpp>

namespace mirv {
  struct StatementVisitor :
    public lib::cyclic_visitor<void,
      boost::mpl::vector<
	ptr<Statement<Base> >::type,
	ptr<LeafStatement>::type,
	ptr<InnerStatement>::type,
	ptr<Statement<Conditional> >::type,
	ptr<Statement<Iterative> >::type,
	ptr<Statement<Mutating> >::type,
	ptr<Statement<Block> >::type,
	ptr<Statement<SingleBlock> >::type,
	ptr<Statement<DualBlock> >::type,
	ptr<Statement<IfThen> >::type,
	ptr<Statement<IfElse> >::type,
	ptr<Statement<While> >::type,
	ptr<Statement<DoWhile> >::type,
	ptr<Statement<Switch> >::type,
	ptr<Statement<Case> >::type,
	ptr<Statement<CaseBlock> >::type,
	ptr<Statement<Before> >::type,
	ptr<Statement<After> >::type,
	ptr<Statement<Goto> >::type,
	ptr<Statement<Return> >::type
	> > {
  public:
    virtual result_type visit(ptr<Statement<Base> >::type);
    virtual result_type visit(ptr<LeafStatement>::type);
    virtual result_type visit(ptr<InnerStatement>::type);
    virtual result_type visit(ptr<Statement<Conditional> >::type);
    virtual result_type visit(ptr<Statement<Iterative> >::type);
    virtual result_type visit(ptr<Statement<Mutating> >::type);
    virtual result_type visit(ptr<Statement<Block> >::type);
    virtual result_type visit(ptr<Statement<SingleBlock> >::type);
    virtual result_type visit(ptr<Statement<DualBlock> >::type);
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
	ptr<Expression<Base> >::type,
	ptr<InnerExpression>::type,
	ptr<InnerExpressionBase>::type,
	ptr<LeafExpression>::type,
	ptr<Expression<Arithmetic> >::type,
	ptr<Expression<Logical> >::type,
	ptr<Expression<Bitwise> >::type,
	ptr<Expression<Ref> >::type,
	ptr<Expression<Commutative> >::type,
	ptr<Expression<Associative> >::type,
	ptr<Expression<Transitive> >::type,
	ptr<Expression<Reflexive> >::type,

	ptr<Expression<Unary> >::type,
	ptr<Expression<Binary> >::type,

	ptr<Expression<Add> >::type,
	ptr<Expression<Subtract> >::type,
	ptr<Expression<Divide> >::type,
	ptr<Expression<Modulus> >::type,
	ptr<Expression<Multiply> >::type,
	ptr<Expression<Negate> >::type,
	ptr<Expression<LogicalAnd> >::type,

	ptr<Expression<LogicalOr> >::type,
	ptr<Expression<LogicalNot> >::type,
	ptr<Expression<BitwiseAnd> >::type,
	ptr<Expression<BitwiseOr> >::type,
	ptr<Expression<BitwiseComplement> >::type,

	ptr<Expression<Reference<Variable> > >::type
	> > {
  public:
    virtual result_type visit(ptr<Expression<Base> >::type);
    virtual result_type visit(ptr<InnerExpression>::type);
    virtual result_type visit(ptr<InnerExpressionBase>::type);
    virtual result_type visit(ptr<LeafExpression>::type);
    virtual result_type visit(ptr<Expression<Arithmetic> >::type);
    virtual result_type visit(ptr<Expression<Logical> >::type);
    virtual result_type visit(ptr<Expression<Bitwise> >::type);
    virtual result_type visit(ptr<Expression<Ref> >::type);
    virtual result_type visit(ptr<Expression<Commutative> >::type);
    virtual result_type visit(ptr<Expression<Associative> >::type);
    virtual result_type visit(ptr<Expression<Transitive> >::type);
    virtual result_type visit(ptr<Expression<Reflexive> >::type);
    virtual result_type visit(ptr<Expression<Unary> >::type);
    virtual result_type visit(ptr<Expression<Binary> >::type);
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
