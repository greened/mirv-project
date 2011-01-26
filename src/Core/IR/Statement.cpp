#include <mirv/Core/Filter/StatementVisitor.hpp>
#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>

namespace mirv{
  void
  Visitable<
    Statement<Base>,
    StatementVisitor,
    boost::mpl::empty_base
    >::accept(StatementVisitor &V) {
    error("Statement<Base>::accept called");
  }
}
