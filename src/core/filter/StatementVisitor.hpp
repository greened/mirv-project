#ifndef mirv_core_filter_StatementVisitor_hpp
#define mirv_core_filter_StatementVisitor_hpp

#include <Visitor.hpp>
 
#include <mirv/core/ir/StatementFwd.hpp>
#include <mirv/core/ir/ControlFwd.hpp>
#include <mirv/core/ir/MutatingFwd.hpp>

#include <mirv/core/ir/ExpressionFwd.hpp>
#include <mirv/core/ir/ArithmeticFwd.hpp>
#include <mirv/core/ir/LogicalFwd.hpp>
#include <mirv/core/ir/BitwiseFwd.hpp>
#include <mirv/core/ir/RelationalFwd.hpp>
#include <mirv/core/ir/ReferenceFwd.hpp>

#include <mirv/core/ir/VariableFwd.hpp>

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
	ptr<Statement<Assignment> >::type
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
  };
}

#endif
