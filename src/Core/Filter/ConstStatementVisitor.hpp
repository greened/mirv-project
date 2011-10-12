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
	ptr<Statement<Base> >::const_type,
	ptr<LeafStatement>::const_type,
	ptr<InnerStatement>::const_type,
	ptr<Statement<Conditional> >::const_type,
	ptr<Statement<Iterative> >::const_type,
	ptr<Statement<Mutating> >::const_type,
	ptr<Statement<Block> >::const_type,
	ptr<Statement<SingleBlock> >::const_type,
	ptr<Statement<DualBlock> >::const_type,
	ptr<Statement<IfThen> >::const_type,
	ptr<Statement<IfElse> >::const_type,
	ptr<Statement<While> >::const_type,
	ptr<Statement<DoWhile> >::const_type,
	ptr<Statement<Switch> >::const_type,
	ptr<Statement<Case> >::const_type,
	ptr<Statement<CaseBlock> >::const_type,
	ptr<Statement<Before> >::const_type,
	ptr<Statement<After> >::const_type,
	ptr<Statement<Goto> >::const_type,
	ptr<Statement<Return> >::const_type,
	ptr<Statement<Phi> >::const_type,
	ptr<Statement<Store> >::const_type,
	ptr<Statement<Call> >::const_type,
	ptr<Statement<Allocate> >::const_type
	> > {
  public:
    virtual result_type visit(ptr<Statement<Base> >::const_type);
    virtual result_type visit(ptr<LeafStatement>::const_type);
    virtual result_type visit(ptr<InnerStatement>::const_type);
    virtual result_type visit(ptr<Statement<Conditional> >::const_type);
    virtual result_type visit(ptr<Statement<Iterative> >::const_type);
    virtual result_type visit(ptr<Statement<Mutating> >::const_type);
    virtual result_type visit(ptr<Statement<Block> >::const_type);
    virtual result_type visit(ptr<Statement<SingleBlock> >::const_type);
    virtual result_type visit(ptr<Statement<DualBlock> >::const_type);
    virtual result_type visit(ptr<Statement<IfThen> >::const_type);
    virtual result_type visit(ptr<Statement<IfElse> >::const_type);
    virtual result_type visit(ptr<Statement<While> >::const_type);
    virtual result_type visit(ptr<Statement<DoWhile> >::const_type);
    virtual result_type visit(ptr<Statement<Switch> >::const_type);
    virtual result_type visit(ptr<Statement<Case> >::const_type);
    virtual result_type visit(ptr<Statement<CaseBlock> >::const_type);
    virtual result_type visit(ptr<Statement<Before> >::const_type);
    virtual result_type visit(ptr<Statement<After> >::const_type);
    virtual result_type visit(ptr<Statement<Goto> >::const_type);
    virtual result_type visit(ptr<Statement<Return> >::const_type);
    virtual result_type visit(ptr<Statement<Phi> >::const_type);
    virtual result_type visit(ptr<Statement<Store> >::const_type);
    virtual result_type visit(ptr<Statement<Call> >::const_type);
    virtual result_type visit(ptr<Statement<Allocate> >::const_type);
  };
}

#endif
