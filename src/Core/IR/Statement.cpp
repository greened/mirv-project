#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Mutating.hpp>

namespace mirv{
  void Statement<Base>::accept(StatementVisitor &V) {
    error("Statement<Base>::accept called");
  }
  void InnerStatement::accept(StatementVisitor &V) {
    error("InnerStatement::accept called");
  }

  void LeafStatement::accept(StatementVisitor &V) {
    error("LeafStatement::accept called");
  }
}
