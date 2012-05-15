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
	ptr<const Expression<Base> >,
	ptr<const InnerExpression>,
	ptr<const InnerExpressionBase>,
	ptr<const LeafExpression>,
	ptr<const Expression<Arithmetic> >,
	ptr<const Expression<Logical> >,
	ptr<const Expression<Bitwise> >,
	ptr<const Expression<Ref> >,
	ptr<const Expression<Commutative> >,
	ptr<const Expression<Associative> >,
	ptr<const Expression<Transitive> >,
	ptr<const Expression<Reflexive> >,

	ptr<const Expression<Unary> >,
	ptr<const Expression<Binary> >,

	ptr<const Expression<Add> >,
	ptr<const Expression<Subtract> >,
	ptr<const Expression<Divide> >,
	ptr<const Expression<Modulus> >,
	ptr<const Expression<Multiply> >,
	ptr<const Expression<Negate> >,

	ptr<const Expression<LogicalAnd> >,
	ptr<const Expression<LogicalOr> >,
	ptr<const Expression<LogicalNot> >,

	ptr<const Expression<BitwiseAnd> >,
	ptr<const Expression<BitwiseOr> >,
	ptr<const Expression<BitwiseXor> >,
	ptr<const Expression<BitwiseComplement> >,
	ptr<const Expression<ShiftLeft> >,
	ptr<const Expression<ArithmeticShiftRight> >,
	ptr<const Expression<LogicalShiftRight> >,

	ptr<const Expression<LessThan> >,
	ptr<const Expression<LessThanOrEqual> >,
	ptr<const Expression<Equal> >,
	ptr<const Expression<NotEqual> >,
	ptr<const Expression<GreaterThanOrEqual> >,
	ptr<const Expression<GreaterThan> >,

	ptr<const Expression<Reference<Variable> > >,
	ptr<const Expression<Reference<Function> > >,
	ptr<const Expression<Reference<Constant<Base> > > >,
	ptr<const Expression<Load> >,
	ptr<const Expression<TuplePointer> >
	> > {
  public:
    virtual result_type visit(ptr<const Expression<Base> >);
    virtual result_type visit(ptr<const InnerExpression>);
    virtual result_type visit(ptr<const InnerExpressionBase>);
    virtual result_type visit(ptr<const LeafExpression>);
    virtual result_type visit(ptr<const Expression<Arithmetic> >);
    virtual result_type visit(ptr<const Expression<Logical> >);
    virtual result_type visit(ptr<const Expression<Bitwise> >);
    virtual result_type visit(ptr<const Expression<Relational> >);
    virtual result_type visit(ptr<const Expression<Ref> >);
    virtual result_type visit(ptr<const Expression<Commutative> >);
    virtual result_type visit(ptr<const Expression<Associative> >);
    virtual result_type visit(ptr<const Expression<Transitive> >);
    virtual result_type visit(ptr<const Expression<Reflexive> >);
    virtual result_type visit(ptr<const Expression<Unary> >);
    virtual result_type visit(ptr<const Expression<Binary> >);
    virtual result_type visit(ptr<const Expression<Add> >);
    virtual result_type visit(ptr<const Expression<Subtract> >);
    virtual result_type visit(ptr<const Expression<Divide> >);
    virtual result_type visit(ptr<const Expression<Multiply> >);
    virtual result_type visit(ptr<const Expression<Modulus> >);
    virtual result_type visit(ptr<const Expression<Negate> >);
    virtual result_type visit(ptr<const Expression<LogicalAnd> >);
    virtual result_type visit(ptr<const Expression<LogicalOr> >);
    virtual result_type visit(ptr<const Expression<LogicalNot> >);
    virtual result_type visit(ptr<const Expression<BitwiseAnd> >);
    virtual result_type visit(ptr<const Expression<BitwiseOr> >);
    virtual result_type visit(ptr<const Expression<BitwiseXor> >);
    virtual result_type visit(ptr<const Expression<BitwiseComplement> >);
    virtual result_type visit(ptr<const Expression<ShiftLeft> >);
    virtual result_type visit(ptr<const Expression<ArithmeticShiftRight> >);
    virtual result_type visit(ptr<const Expression<LogicalShiftRight> >);
    virtual result_type visit(ptr<const Expression<LessThan> >);
    virtual result_type visit(ptr<const Expression<LessThanOrEqual> >);
    virtual result_type visit(ptr<const Expression<Equal> >);
    virtual result_type visit(ptr<const Expression<NotEqual> >);
    virtual result_type visit(ptr<const Expression<GreaterThanOrEqual> >);
    virtual result_type visit(ptr<const Expression<GreaterThan> >);
    virtual result_type visit(ptr<const Expression<Reference<Variable> > >);
    virtual result_type visit(ptr<const Expression<Reference<Function> > >);
    virtual result_type visit(ptr<const Expression<Reference<Constant<Base> > > >);
    virtual result_type visit(ptr<const Expression<Load> >);
    virtual result_type visit(ptr<const Expression<TuplePointer> >);
  };
}

#endif
