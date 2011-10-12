#ifndef mirv_Core_Filter_ConstExpressionVisitor_hpp
#define mirv_Core_Filter_ConstExpressionVisitor_hpp

#include <Visitor.hpp>
 
#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/ArithmeticFwd.hpp>
#include <mirv/Core/IR/LogicalFwd.hpp>
#include <mirv/Core/IR/BitwiseFwd.hpp>
#include <mirv/Core/IR/RelationalFwd.hpp>
#include <mirv/Core/IR/ReferenceFwd.hpp>

#include <mirv/Core/IR/ConstantFwd.hpp>
#include <mirv/Core/IR/FunctionFwd.hpp>
#include <mirv/Core/IR/VariableFwd.hpp>

#include <boost/mpl/vector.hpp>

namespace mirv {
  /// This is the base class for all visitors that know about
  /// expressions.  This includes flows, visitor actions and anything
  /// else that wants to examine or operate on expressions.
  struct ConstExpressionVisitor :
    public lib::cyclic_visitor<void,
      boost::mpl::vector<
	ptr<Expression<Base> >::const_type,
	ptr<InnerExpression>::const_type,
	ptr<InnerExpressionBase>::const_type,
	ptr<LeafExpression>::const_type,
	ptr<Expression<Arithmetic> >::const_type,
	ptr<Expression<Logical> >::const_type,
	ptr<Expression<Bitwise> >::const_type,
	ptr<Expression<Ref> >::const_type,
	ptr<Expression<Commutative> >::const_type,
	ptr<Expression<Associative> >::const_type,
	ptr<Expression<Transitive> >::const_type,
	ptr<Expression<Reflexive> >::const_type,

	ptr<Expression<Unary> >::const_type,
	ptr<Expression<Binary> >::const_type,

	ptr<Expression<Add> >::const_type,
	ptr<Expression<Subtract> >::const_type,
	ptr<Expression<Divide> >::const_type,
	ptr<Expression<Modulus> >::const_type,
	ptr<Expression<Multiply> >::const_type,
	ptr<Expression<Negate> >::const_type,

	ptr<Expression<LogicalAnd> >::const_type,
	ptr<Expression<LogicalOr> >::const_type,
	ptr<Expression<LogicalNot> >::const_type,

	ptr<Expression<BitwiseAnd> >::const_type,
	ptr<Expression<BitwiseOr> >::const_type,
	ptr<Expression<BitwiseXor> >::const_type,
	ptr<Expression<BitwiseComplement> >::const_type,
	ptr<Expression<ShiftLeft> >::const_type,
	ptr<Expression<ArithmeticShiftRight> >::const_type,
	ptr<Expression<LogicalShiftRight> >::const_type,

	ptr<Expression<LessThan> >::const_type,
	ptr<Expression<LessThanOrEqual> >::const_type,
	ptr<Expression<Equal> >::const_type,
	ptr<Expression<NotEqual> >::const_type,
	ptr<Expression<GreaterThanOrEqual> >::const_type,
	ptr<Expression<GreaterThan> >::const_type,

	ptr<Expression<Reference<Variable> > >::const_type,
	ptr<Expression<Reference<Function> > >::const_type,
	ptr<Expression<Reference<Constant<Base> > > >::const_type,
	ptr<Expression<Load> >::const_type,
	ptr<Expression<TuplePointer> >::const_type
	> > {
  public:
    virtual result_type visit(ptr<Expression<Base> >::const_type);
    virtual result_type visit(ptr<InnerExpression>::const_type);
    virtual result_type visit(ptr<InnerExpressionBase>::const_type);
    virtual result_type visit(ptr<LeafExpression>::const_type);
    virtual result_type visit(ptr<Expression<Arithmetic> >::const_type);
    virtual result_type visit(ptr<Expression<Logical> >::const_type);
    virtual result_type visit(ptr<Expression<Bitwise> >::const_type);
    virtual result_type visit(ptr<Expression<Relational> >::const_type);
    virtual result_type visit(ptr<Expression<Ref> >::const_type);
    virtual result_type visit(ptr<Expression<Commutative> >::const_type);
    virtual result_type visit(ptr<Expression<Associative> >::const_type);
    virtual result_type visit(ptr<Expression<Transitive> >::const_type);
    virtual result_type visit(ptr<Expression<Reflexive> >::const_type);
    virtual result_type visit(ptr<Expression<Unary> >::const_type);
    virtual result_type visit(ptr<Expression<Binary> >::const_type);
    virtual result_type visit(ptr<Expression<Add> >::const_type);
    virtual result_type visit(ptr<Expression<Subtract> >::const_type);
    virtual result_type visit(ptr<Expression<Divide> >::const_type);
    virtual result_type visit(ptr<Expression<Multiply> >::const_type);
    virtual result_type visit(ptr<Expression<Modulus> >::const_type);
    virtual result_type visit(ptr<Expression<Negate> >::const_type);
    virtual result_type visit(ptr<Expression<LogicalAnd> >::const_type);
    virtual result_type visit(ptr<Expression<LogicalOr> >::const_type);
    virtual result_type visit(ptr<Expression<LogicalNot> >::const_type);
    virtual result_type visit(ptr<Expression<BitwiseAnd> >::const_type);
    virtual result_type visit(ptr<Expression<BitwiseOr> >::const_type);
    virtual result_type visit(ptr<Expression<BitwiseXor> >::const_type);
    virtual result_type visit(ptr<Expression<BitwiseComplement> >::const_type);
    virtual result_type visit(ptr<Expression<ShiftLeft> >::const_type);
    virtual result_type visit(ptr<Expression<ArithmeticShiftRight> >::const_type);
    virtual result_type visit(ptr<Expression<LogicalShiftRight> >::const_type);
    virtual result_type visit(ptr<Expression<LessThan> >::const_type);
    virtual result_type visit(ptr<Expression<LessThanOrEqual> >::const_type);
    virtual result_type visit(ptr<Expression<Equal> >::const_type);
    virtual result_type visit(ptr<Expression<NotEqual> >::const_type);
    virtual result_type visit(ptr<Expression<GreaterThanOrEqual> >::const_type);
    virtual result_type visit(ptr<Expression<GreaterThan> >::const_type);
    virtual result_type visit(ptr<Expression<Reference<Variable> > >::const_type);
    virtual result_type visit(ptr<Expression<Reference<Function> > >::const_type);
    virtual result_type visit(ptr<Expression<Reference<Constant<Base> > > >::const_type);
    virtual result_type visit(ptr<Expression<Load> >::const_type);
    virtual result_type visit(ptr<Expression<TuplePointer> >::const_type);
  };
}

#endif
