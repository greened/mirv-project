#ifndef mirv_Core_Filter_ConstStatementVisitor_hpp
#define mirv_Core_Filter_ConstStatementVisitor_hpp

#include <Visitor.hpp>
 
#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <mirv/Core/IR/StatementFwd.hpp>
#include <mirv/Core/IR/ControlFwd.hpp>
#include <mirv/Core/IR/MutatingFwd.hpp>

#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/ArithmeticFwd.hpp>
#include <mirv/Core/IR/LogicalFwd.hpp>
#include <mirv/Core/IR/BitwiseFwd.hpp>
#include <mirv/Core/IR/RelationalFwd.hpp>
#include <mirv/Core/IR/ReferenceFwd.hpp>

#include <mirv/Core/IR/VariableFwd.hpp>

#include <boost/mpl/vector.hpp>

namespace mirv {
  /// This is the base class for all visitors of statements.
  struct ConstStatementVisitor :
    public lib::cyclic_visitor<void,
      boost::mpl::vector<
	ptr<const Statement<Base> >,
	ptr<const LeafStatement>,
	ptr<const InnerStatement>,
	ptr<const Statement<Conditional> >,
	ptr<const Statement<Iterative> >,
	ptr<const Statement<Mutating> >,
	ptr<const Statement<Block> >,
	ptr<const Statement<SingleBlock> >,
	ptr<const Statement<DualBlock> >,
	ptr<const Statement<IfThen> >,
	ptr<const Statement<IfElse> >,
	ptr<const Statement<While> >,
	ptr<const Statement<DoWhile> >,
	ptr<const Statement<Switch> >,
	ptr<const Statement<Case> >,
	ptr<const Statement<CaseBlock> >,
	ptr<const Statement<Before> >,
	ptr<const Statement<After> >,
	ptr<const Statement<Goto> >,
	ptr<const Statement<Return> >,
	ptr<const Statement<Phi> >,
	ptr<const Statement<Store> >,
	ptr<const Statement<Call> >,
	ptr<const Statement<Allocate> >
	> > {
  public:
    virtual result_type visit(ptr<const Statement<Base> >);
    virtual result_type visit(ptr<const LeafStatement>);
    virtual result_type visit(ptr<const InnerStatement>);
    virtual result_type visit(ptr<const Statement<Conditional> >);
    virtual result_type visit(ptr<const Statement<Iterative> >);
    virtual result_type visit(ptr<const Statement<Mutating> >);
    virtual result_type visit(ptr<const Statement<Block> >);
    virtual result_type visit(ptr<const Statement<SingleBlock> >);
    virtual result_type visit(ptr<const Statement<DualBlock> >);
    virtual result_type visit(ptr<const Statement<IfThen> >);
    virtual result_type visit(ptr<const Statement<IfElse> >);
    virtual result_type visit(ptr<const Statement<While> >);
    virtual result_type visit(ptr<const Statement<DoWhile> >);
    virtual result_type visit(ptr<const Statement<Switch> >);
    virtual result_type visit(ptr<const Statement<Case> >);
    virtual result_type visit(ptr<const Statement<CaseBlock> >);
    virtual result_type visit(ptr<const Statement<Before> >);
    virtual result_type visit(ptr<const Statement<After> >);
    virtual result_type visit(ptr<const Statement<Goto> >);
    virtual result_type visit(ptr<const Statement<Return> >);
    virtual result_type visit(ptr<const Statement<Phi> >);
    virtual result_type visit(ptr<const Statement<Store> >);
    virtual result_type visit(ptr<const Statement<Call> >);
    virtual result_type visit(ptr<const Statement<Allocate> >);
  };
}

#endif
