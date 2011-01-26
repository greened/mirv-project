#ifndef mirv_Core_Filter_StatementVisitor_hpp
#define mirv_Core_Filter_StatementVisitor_hpp

#include <Visitor.hpp>
 
#include <mirv/Core/IR/Base.hpp>
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
	ptr<Statement<Return> >::type,
	ptr<Statement<Assignment> >::type,
	ptr<Statement<Call> >::type
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
    virtual result_type visit(ptr<Statement<Assignment> >::type);
    virtual result_type visit(ptr<Statement<Call> >::type);
  };
}

#endif
