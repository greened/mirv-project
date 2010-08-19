#ifndef mirv_Core_Filter_ExpressionVisitor_hpp
#define mirv_Core_Filter_ExpressionVisitor_hpp

#include <Visitor.hpp>
 
#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/ArithmeticFwd.hpp>
#include <mirv/Core/IR/LogicalFwd.hpp>
#include <mirv/Core/IR/BitwiseFwd.hpp>
#include <mirv/Core/IR/RelationalFwd.hpp>
#include <mirv/Core/IR/ReferenceFwd.hpp>

#include <mirv/Core/IR/VariableFwd.hpp>

#include <boost/mpl/vector.hpp>

namespace mirv {
  /// This is the base class for all visitors that know about
  /// expressions.  This includes flows, visitor actions and anything
  /// else that wants to examine or operate on expressions.
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

	ptr<Expression<LessThan> >::type,
	ptr<Expression<LessThanOrEqual> >::type,
	ptr<Expression<Equal> >::type,
	ptr<Expression<NotEqual> >::type,
	ptr<Expression<GreaterThanOrEqual> >::type,
	ptr<Expression<GreaterThan> >::type,

	ptr<Expression<Reference<Variable> > >::type,
	ptr<Expression<Reference<Function> > >::type
	> > {
  public:
    virtual result_type visit(ptr<Expression<Base> >::type);
    virtual result_type visit(ptr<InnerExpression>::type);
    virtual result_type visit(ptr<InnerExpressionBase>::type);
    virtual result_type visit(ptr<LeafExpression>::type);
    virtual result_type visit(ptr<Expression<Arithmetic> >::type);
    virtual result_type visit(ptr<Expression<Logical> >::type);
    virtual result_type visit(ptr<Expression<Bitwise> >::type);
    virtual result_type visit(ptr<Expression<Relational> >::type);
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
    virtual result_type visit(ptr<Expression<LessThan> >::type);
    virtual result_type visit(ptr<Expression<LessThanOrEqual> >::type);
    virtual result_type visit(ptr<Expression<Equal> >::type);
    virtual result_type visit(ptr<Expression<NotEqual> >::type);
    virtual result_type visit(ptr<Expression<GreaterThanOrEqual> >::type);
    virtual result_type visit(ptr<Expression<GreaterThan> >::type);
    virtual result_type visit(ptr<Expression<Reference<Variable> > >::type);
    virtual result_type visit(ptr<Expression<Reference<Function> > >::type);
  };
}

#endif
