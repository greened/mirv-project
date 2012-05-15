#ifndef mirv_Core_Filter_StatementVisitor_hpp
#define mirv_Core_Filter_StatementVisitor_hpp

#include <Visitor.hpp>

#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <mirv/Core/IR/StatementFwd.hpp>
#include <mirv/Core/IR/ControlFwd.hpp>
#include <mirv/Core/IR/MutatingFwd.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/ArithmeticFwd.hpp>
#include <mirv/Core/IR/LogicalFwd.hpp>
#include <mirv/Core/IR/BitwiseFwd.hpp>
#include <mirv/Core/IR/RelationalFwd.hpp>
#include <mirv/Core/IR/ReferenceFwd.hpp>

#include <mirv/Core/IR/VariableFwd.hpp>
#include <mirv/Core/Memory/Heap.hpp>

#include <boost/mpl/vector.hpp>

namespace mirv {
  /// This is the base class for all visitors of statements.
  struct StatementVisitor :
    public lib::cyclic_visitor<void,
      boost::mpl::vector<
	ptr<Statement<Base> >,
	ptr<LeafStatement>,
	ptr<InnerStatement>,
	ptr<Statement<Conditional> >,
	ptr<Statement<Iterative> >,
	ptr<Statement<Mutating> >,
	ptr<Statement<Block> >,
	ptr<Statement<SingleBlock> >,
	ptr<Statement<DualBlock> >,
	ptr<Statement<IfThen> >,
	ptr<Statement<IfElse> >,
	ptr<Statement<While> >,
	ptr<Statement<DoWhile> >,
	ptr<Statement<Switch> >,
	ptr<Statement<Case> >,
	ptr<Statement<CaseBlock> >,
	ptr<Statement<Before> >,
	ptr<Statement<After> >,
	ptr<Statement<Goto> >,
	ptr<Statement<Return> >,
	ptr<Statement<Phi> >,
	ptr<Statement<Store> >,
	ptr<Statement<Call> >,
	ptr<Statement<Allocate> >
	> > {
  public:
    virtual result_type visit(ptr<Statement<Base> >);
    virtual result_type visit(ptr<LeafStatement>);
    virtual result_type visit(ptr<InnerStatement>);
    virtual result_type visit(ptr<Statement<Conditional> >);
    virtual result_type visit(ptr<Statement<Iterative> >);
    virtual result_type visit(ptr<Statement<Mutating> >);
    virtual result_type visit(ptr<Statement<Block> >);
    virtual result_type visit(ptr<Statement<SingleBlock> >);
    virtual result_type visit(ptr<Statement<DualBlock> >);
    virtual result_type visit(ptr<Statement<IfThen> >);
    virtual result_type visit(ptr<Statement<IfElse> >);
    virtual result_type visit(ptr<Statement<While> >);
    virtual result_type visit(ptr<Statement<DoWhile> >);
    virtual result_type visit(ptr<Statement<Switch> >);
    virtual result_type visit(ptr<Statement<Case> >);
    virtual result_type visit(ptr<Statement<CaseBlock> >);
    virtual result_type visit(ptr<Statement<Before> >);
    virtual result_type visit(ptr<Statement<After> >);
    virtual result_type visit(ptr<Statement<Goto> >);
    virtual result_type visit(ptr<Statement<Return> >);
    virtual result_type visit(ptr<Statement<Phi> >);
    virtual result_type visit(ptr<Statement<Store> >);
    virtual result_type visit(ptr<Statement<Call> >);
    virtual result_type visit(ptr<Statement<Allocate> >);
  };
}

#endif
