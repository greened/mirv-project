#include <mirv/Core/Filter/StatementVisitor.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Expression.hpp>

namespace mirv {
  Statement<Base> *detail::BlockInterface::cloneImpl(void)
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

  void detail::BlockInterface::setParents(void)
  {
    for (iterator i = begin();
         i != end();
         ++i) {
      (*i)->setParent(getSharedHandle());
    }
  }

  Statement<Base> *detail::IfThenInterface::cloneImpl(void)
  {
    ptr<Statement<IfThen> >::type stmt(Statement<IfThen>::make(
                                         getCondition()->clone(),
                                         getChildStatement()->clone()));
    Statement<IfThen> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::IfThenInterface::setParents(void)
  {
    getCondition()->setParent(getSharedHandle());
    getChildStatement()->setParent(getSharedHandle());
  }

  Statement<Base> *detail::IfElseInterface::cloneImpl(void)
  {
    ptr<Statement<IfElse> >::type stmt(Statement<IfElse>::make(
                                         getCondition()->clone(),
                                         getLeftChildStatement()->clone(),
                                         getRightChildStatement()->clone()));
    Statement<IfElse> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::IfElseInterface::setParents(void)
  {
    getCondition()->setParent(getSharedHandle());
    getLeftChildStatement()->setParent(getSharedHandle());
    getRightChildStatement()->setParent(getSharedHandle());
  }

  Statement<Base> *detail::DoWhileInterface::cloneImpl(void)
  {
    ptr<Statement<DoWhile> >::type stmt(Statement<DoWhile>::make(
                                          getCondition()->clone(),
                                          getChildStatement()->clone()));
    Statement<DoWhile> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::DoWhileInterface::setParents(void)
  {
    getCondition()->setParent(getSharedHandle());
    getChildStatement()->setParent(getSharedHandle());
  }

  Statement<Base> *detail::WhileInterface::cloneImpl(void)
  {
    ptr<Statement<While> >::type stmt(Statement<While>::make(
                                        getCondition()->clone(),
                                        getChildStatement()->clone()));
    Statement<While> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::WhileInterface::setParents(void)
  {
    getCondition()->setParent(getSharedHandle());
    getChildStatement()->setParent(getSharedHandle());
  }

  Statement<Base> *detail::CaseInterface::cloneImpl(void)
  {
    ptr<Statement<Case> >::type stmt(Statement<Case>::make(
                                       getCondition()->clone(),
                                           getChildStatement()->clone()));
    Statement<Case> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::CaseInterface::setParents(void)
  {
    getCondition()->setParent(getSharedHandle());
    getChildStatement()->setParent(getSharedHandle());
  }

  void detail::CaseBlockInterface::setParents(void)
  {
    getChildStatement()->setParent(getSharedHandle());
  }

  Statement<Base> *detail::SwitchInterface::cloneImpl(void)
  {
    ptr<Statement<Switch> >::type stmt(Statement<Switch>::make(
                                         getCondition()->clone(),
                                         getChildStatement()->clone()));
    Statement<Switch> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::SwitchInterface::setParents(void)
  {
    getCondition()->setParent(getSharedHandle());
    getChildStatement()->setParent(getSharedHandle());
  }

  Statement<Base> *detail::BeforeInterface::cloneImpl(void)
  {
    ptr<Statement<Before> >::type stmt(Statement<Before>::make(
                                         getLabel()->clone(),
                                         getChildStatement()->clone()));
    Statement<Before> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::BeforeInterface::setParents(void)
  {
    getLabel()->setParent(getSharedHandle());
    getChildStatement()->setParent(getSharedHandle());
  }

  Statement<Base> *detail::AfterInterface::cloneImpl(void)
  {
    ptr<Statement<After> >::type stmt(Statement<After>::make(
                                        getLabel()->clone(),
                                        getChildStatement()->clone()));
    Statement<After> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::AfterInterface::setParents(void)
  {
    getLabel()->setParent(getSharedHandle());
    getChildStatement()->setParent(getSharedHandle());
  }

  Statement<Base> *detail::GotoInterface::cloneImpl(void)
  {
    ptr<Statement<Goto> >::type stmt(Statement<Goto>::make(
                                       getLabel()->clone()));
    Statement<Goto> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::GotoInterface::setParents(void)
  {
    getLabel()->setParent(getSharedHandle());
  }

  Statement<Base> *detail::ReturnInterface::cloneImpl(void)
  {
    ptr<Statement<Return> >::type stmt(Statement<Return>::make());
    Statement<Return> *result = stmt.get();
    stmt.reset();
    return result;
  }
}
