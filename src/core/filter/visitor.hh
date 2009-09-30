#ifndef mirv_core_filter_visitor_hh
#define mirv_core_filter_visitor_hh

#include <visitor.hh>

#include <mirv/ir/control.hh>

#include <mirv/ir/arithmetic.hh>
#include <mirv/ir/logical.hh>

#include <boost/mpl/vector.hpp>

namespace MIRV {
   typedef cyclic_visitor<void,
      boost::mpl::vector<
      BaseStatement,
      Statement<Block>,
      Statement<IfThen>,
      Statement<IfElse>,
      Statement<While>,
      Statement<DoWhile>,
      Statement<Switch>,
      Statement<Case>,
      Statement<CaseBlock>,
      Statement<Before>,
      Statement<After>,
      Statement<Goto>,
      Statement<Return>
      > > StatementVisitor;

   typedef cyclic_visitor<void,
      boost::mpl::vector<
      BaseExpression,
      Expression<Add>,
      Expression<Sub>,
      Expression<Div>,
      Expression<Mod>,
      Expression<Mult>,
      Expression<Neg>,
      Expression<LogicalAnd>,
      Expression<LogicalOr>,
      Expression<LogicalNot>,
      Expression<BitAnd>,
      Expression<BitOr>,
      Expression<BitNot>
      > > ExpressionVisitor;

#define DEFINE_STATEMENT_VISITABLE() \
   DEFINE_CYCLIC_VISITABLE(StatementVisitor);

#define DEFINE_EXPRESSION_VISITABLE() \
   DEFINE_CYCLIC_VISITABLE(ExpressionVisitor);
}

#endif
