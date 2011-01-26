#include <mirv/Core/IR/Function.hpp>

namespace mirv {
  void Function::Interface::statementPushBack(StatementPtr stmt)
  {
    // If the statement is not a block, make it one.
    if (StatementBaseType::empty())  {
      StatementPtr newStmt = (dyn_cast<Statement<Block> >(stmt) ?
                              stmt : boost::static_pointer_cast<Statement<Base> >(mirv::make<Statement<Block> >(stmt)));
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
}
