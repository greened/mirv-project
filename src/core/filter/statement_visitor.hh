#ifndef mirv_core_filter_statement_visitor_hh
#define mirv_core_filter_statement_visitor_hh

#include <visitor.hh>
 
#include <mirv/core/ir/statement_fwd.hh>
#include <mirv/core/ir/control_fwd.hh>
#include <mirv/core/ir/mutating_fwd.hh>

#include <mirv/core/ir/expression_fwd.hh>
#include <mirv/core/ir/arithmetic_fwd.hh>
#include <mirv/core/ir/logical_fwd.hh>
#include <mirv/core/ir/bitwise_fwd.hh>
#include <mirv/core/ir/relational_fwd.hh>
#include <mirv/core/ir/reference_fwd.hh>

#include <mirv/core/ir/variable_fwd.hh>

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
