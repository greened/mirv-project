#include <mirv/Core/IR/Mutating.hpp>

namespace mirv {
  Statement<Assignment> *Assignment::Interface::cloneImpl(void) 
  {
    ptr<Statement<Assignment> >::type stmt(Statement<Assignment>::make(
                                             getLeftExpression()->clone(),
                                             getRightExpression()->clone()));
    Statement<Assignment> *result = stmt.get();
    stmt.reset();
    return result;
  }

  Statement<Call> *Call::Interface::cloneImpl(void) 
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
}
