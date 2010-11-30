#include <mirv/Core/IR/Control.hpp>

namespace mirv {
  Statement<Base> *Block::Interface::cloneImpl(void)
  {
    ptr<Statement<Block> >::type block(Statement<Block>::make());
    for (Statement<Block>::iterator i = begin();
         i != end();
         ++i) {
      block->push_back((*i)->clone());
    }
    Statement<Block> *result = block.get();
    block.reset();
    return result;
  }

  Statement<Base> *IfThen::Interface::cloneImpl(void)
  {
    ptr<Statement<IfThen> >::type stmt(Statement<IfThen>::make(
                                         getCondition()->clone(),
                                         getChildStatement()->clone()));
    Statement<IfThen> *result = stmt.get();
    stmt.reset();
    return result;
  }

  Statement<Base> *IfElse::Interface::cloneImpl(void)
  {
    ptr<Statement<IfElse> >::type stmt(Statement<IfElse>::make(
                                         getCondition()->clone(),
                                         getLeftChildStatement()->clone(),
                                         getRightChildStatement()->clone()));
    Statement<IfElse> *result = stmt.get();
    stmt.reset();
    return result;
  }

  Statement<Base> *DoWhile::Interface::cloneImpl(void)
  {
    ptr<Statement<DoWhile> >::type stmt(Statement<DoWhile>::make(
                                          getCondition()->clone(),
                                          getChildStatement()->clone()));
    Statement<DoWhile> *result = stmt.get();
    stmt.reset();
    return result;
  }

  Statement<Base> *While::Interface::cloneImpl(void)
  {
    ptr<Statement<While> >::type stmt(Statement<While>::make(
                                        getCondition()->clone(),
                                        getChildStatement()->clone()));
    Statement<While> *result = stmt.get();
    stmt.reset();
    return result;
  }

  Statement<Base> *Case::Interface::cloneImpl(void)
  {
    ptr<Statement<Case> >::type stmt(Statement<Case>::make(
                                       getCondition()->clone(),
                                           getChildStatement()->clone()));
    Statement<Case> *result = stmt.get();
    stmt.reset();
    return result;
  }

  Statement<Base> *Switch::Interface::cloneImpl(void)
  {
    ptr<Statement<Switch> >::type stmt(Statement<Switch>::make(
                                         getCondition()->clone(),
                                         getChildStatement()->clone()));
    Statement<Switch> *result = stmt.get();
    stmt.reset();
    return result;
  }

  Statement<Base> *Before::Interface::cloneImpl(void)
  {
    ptr<Statement<Before> >::type stmt(Statement<Before>::make(
                                         getLabel()->clone(),
                                         getChildStatement()->clone()));
    Statement<Before> *result = stmt.get();
    stmt.reset();
    return result;
  }

  Statement<Base> *After::Interface::cloneImpl(void)
  {
    ptr<Statement<After> >::type stmt(Statement<After>::make(
                                        getLabel()->clone(),
                                        getChildStatement()->clone()));
    Statement<After> *result = stmt.get();
    stmt.reset();
    return result;
  }

  Statement<Base> *Goto::Interface::cloneImpl(void)
  {
    ptr<Statement<Goto> >::type stmt(Statement<Goto>::make(
                                       getLabel()->clone()));
    Statement<Goto> *result = stmt.get();
    stmt.reset();
    return result;
  }

  Statement<Base> *Return::Interface::cloneImpl(void)
  {
    ptr<Statement<Return> >::type stmt(Statement<Return>::make());
    Statement<Return> *result = stmt.get();
    stmt.reset();
    return result;
  }
}
