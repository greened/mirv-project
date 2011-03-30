#include <mirv/Core/Filter/StatementVisitor.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>

namespace mirv {
  Function::Interface::Interface(const std::string &n,
                                 TypePtr t,
                                 StatementPtr s)
      : GlobalBaseType(n, t),
          // If the statement is not a block, make it one.
          StatementBaseType(dyn_cast<Statement<Block> >(s) ?
                            s : boost::static_pointer_cast<Statement<Base> >(mirv::make<Statement<Block> >(s))) {}

  void Function::Interface::statementPushBack(StatementPtr stmt)
  {
    // If the statement is not a block, make it one.
    if (StatementBaseType::empty())  {
      StatementPtr newStmt = (
        dyn_cast<Statement<Block> >(stmt) ?
        stmt :
        boost::static_pointer_cast<Statement<Base> >(
          mirv::make<Statement<Block> >(stmt)));

      StatementBaseType::push_back(newStmt);
      newStmt->setParent(this->getSharedHandle());
    }
    else {
      ptr<Statement<Block> >::type block =
        safe_cast<Statement<Block> >(StatementBaseType::front());
      block->push_back(stmt);
      block->setParent(this->getSharedHandle());
    }
  }

  void Function::Interface::variablePushBack(VariablePtr v)
  {
    VariableBaseType::push_back(v); 
    v->setParent(this->getSharedHandle());
  }
}
