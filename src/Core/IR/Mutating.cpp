#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/IR/Mutating.hpp>

namespace mirv {
  Statement<Base> *Phi::Interface::cloneImpl(void) 
  {
    ptr<Statement<Phi> >::type stmt(Statement<Phi>::make(this->target()));
    Statement<Phi>::iterator i = this->begin();
    while (++i != this->end()) {
      stmt->expressionPushBack((*i)->clone());
    }
    Statement<Phi> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void Phi::Interface::setParents(void)
  {
    for (ExpressionIterator i = expressionBegin();
         i != expressionEnd();
         ++i) {
      (*i)->setParent(getSharedHandle());
    }
  }

  Statement<Base> *Assignment::Interface::cloneImpl(void) 
  {
    ptr<Statement<Assignment> >::type stmt(Statement<Assignment>::make(
                                             getLeftExpression()->clone(),
                                             getRightExpression()->clone()));
    Statement<Assignment> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void Assignment::Interface::setParents(void)
  {
    getLeftExpression()->setParent(getSharedHandle());
    getRightExpression()->setParent(getSharedHandle());
  }

  Statement<Base> *Call::Interface::cloneImpl(void) 
  {
    ptr<Statement<Call> >::type stmt(Statement<Call>::make(this->function()->clone()));
    Statement<Call>::iterator i = this->begin();
    while (++i != this->end()) {
      stmt->expressionPushBack((*i)->clone());
    }
    Statement<Call> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void Call::Interface::setParents(void) 
  {
    for (ExpressionIterator i = expressionBegin();
         i != expressionEnd();
         ++i) {
      (*i)->setParent(getSharedHandle());
    }
  }

  Statement<Base> *Allocate::Interface::cloneImpl(void) 
  {
    ptr<Statement<Allocate> >::type stmt(Statement<Allocate>::make(
                                           getLeftExpression()->clone(),
                                           getRightExpression()->clone(),
                                           type()));
    Statement<Allocate> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void Allocate::Interface::setParents(void)
  {
    getLeftExpression()->setParent(getSharedHandle());
    getRightExpression()->setParent(getSharedHandle());
  }
}
