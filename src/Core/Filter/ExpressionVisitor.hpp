#ifndef mirv_Core_Filter_ExpressionVisitor_hpp
#define mirv_Core_Filter_ExpressionVisitor_hpp

#include <Visitor.hpp>
 
#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/IR/Visitable.hpp>

#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/FunctionFwd.hpp>
#include <mirv/Core/IR/ArithmeticFwd.hpp>
#include <mirv/Core/IR/LogicalFwd.hpp>
#include <mirv/Core/IR/BitwiseFwd.hpp>
#include <mirv/Core/IR/RelationalFwd.hpp>
#include <mirv/Core/IR/ReferenceFwd.hpp>

#include <mirv/Core/IR/ConstantFwd.hpp>
#include <mirv/Core/IR/FunctionFwd.hpp>
#include <mirv/Core/IR/VariableFwd.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <boost/mpl/vector.hpp>

namespace mirv {
  /// This is the base class for all visitors that know about
  /// expressions.  This includes flows, visitor actions and anything
  /// else that wants to examine or operate on expressions.
  struct ExpressionVisitor :
    public lib::cyclic_visitor<void,
      boost::mpl::vector<
	ptr<Expression<Base> >,
	ptr<InnerExpression>,
	ptr<InnerExpressionBase>,
	ptr<LeafExpression>,
	ptr<Expression<Arithmetic> >,
	ptr<Expression<Logical> >,
	ptr<Expression<Bitwise> >,
	ptr<Expression<Ref> >,
	ptr<Expression<Commutative> >,
	ptr<Expression<Associative> >,
	ptr<Expression<Transitive> >,
	ptr<Expression<Reflexive> >,

	ptr<Expression<Unary> >,
	ptr<Expression<Binary> >,

	ptr<Expression<Add> >,
	ptr<Expression<Subtract> >,
	ptr<Expression<Divide> >,
	ptr<Expression<Modulus> >,
	ptr<Expression<Multiply> >,
	ptr<Expression<Negate> >,

	ptr<Expression<LogicalAnd> >,
	ptr<Expression<LogicalOr> >,
	ptr<Expression<LogicalNot> >,

	ptr<Expression<BitwiseAnd> >,
	ptr<Expression<BitwiseOr> >,
	ptr<Expression<BitwiseXor> >,
	ptr<Expression<BitwiseComplement> >,
	ptr<Expression<ShiftLeft> >,
	ptr<Expression<ArithmeticShiftRight> >,
	ptr<Expression<LogicalShiftRight> >,

	ptr<Expression<LessThan> >,
	ptr<Expression<LessThanOrEqual> >,
	ptr<Expression<Equal> >,
	ptr<Expression<NotEqual> >,
	ptr<Expression<GreaterThanOrEqual> >,
	ptr<Expression<GreaterThan> >,

	ptr<Expression<Reference<Variable> > >,
	ptr<Expression<Reference<Function> > >,
	ptr<Expression<Reference<Constant<Base> > > >,
	ptr<Expression<Load> >,

	ptr<Expression<TuplePointer> >
	> > {
  public:
    virtual result_type visit(ptr<Expression<Base> >);
    virtual result_type visit(ptr<InnerExpression>);
    virtual result_type visit(ptr<InnerExpressionBase>);
    virtual result_type visit(ptr<LeafExpression>);
    virtual result_type visit(ptr<Expression<Arithmetic> >);
    virtual result_type visit(ptr<Expression<Logical> >);
    virtual result_type visit(ptr<Expression<Bitwise> >);
    virtual result_type visit(ptr<Expression<Relational> >);
    virtual result_type visit(ptr<Expression<Ref> >);
    virtual result_type visit(ptr<Expression<Commutative> >);
    virtual result_type visit(ptr<Expression<Associative> >);
    virtual result_type visit(ptr<Expression<Transitive> >);
    virtual result_type visit(ptr<Expression<Reflexive> >);
    virtual result_type visit(ptr<Expression<Unary> >);
    virtual result_type visit(ptr<Expression<Binary> >);
    virtual result_type visit(ptr<Expression<Add> >);
    virtual result_type visit(ptr<Expression<Subtract> >);
    virtual result_type visit(ptr<Expression<Divide> >);
    virtual result_type visit(ptr<Expression<Multiply> >);
    virtual result_type visit(ptr<Expression<Modulus> >);
    virtual result_type visit(ptr<Expression<Negate> >);
    virtual result_type visit(ptr<Expression<LogicalAnd> >);
    virtual result_type visit(ptr<Expression<LogicalOr> >);
    virtual result_type visit(ptr<Expression<LogicalNot> >);
    virtual result_type visit(ptr<Expression<BitwiseAnd> >);
    virtual result_type visit(ptr<Expression<BitwiseOr> >);
    virtual result_type visit(ptr<Expression<BitwiseXor> >);
    virtual result_type visit(ptr<Expression<BitwiseComplement> >);
    virtual result_type visit(ptr<Expression<ShiftLeft> >);
    virtual result_type visit(ptr<Expression<ArithmeticShiftRight> >);
    virtual result_type visit(ptr<Expression<LogicalShiftRight> >);
    virtual result_type visit(ptr<Expression<LessThan> >);
    virtual result_type visit(ptr<Expression<LessThanOrEqual> >);
    virtual result_type visit(ptr<Expression<Equal> >);
    virtual result_type visit(ptr<Expression<NotEqual> >);
    virtual result_type visit(ptr<Expression<GreaterThanOrEqual> >);
    virtual result_type visit(ptr<Expression<GreaterThan> >);
    virtual result_type visit(ptr<Expression<Reference<Variable> > >);
    virtual result_type visit(ptr<Expression<Reference<Function> > >);
    virtual result_type visit(ptr<Expression<Reference<Constant<Base> > > >);
    virtual result_type visit(ptr<Expression<Load> >);
    virtual result_type visit(ptr<Expression<TuplePointer> >);
  };
}

#endif
