#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/Utility/Printer.hpp>

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

  Statement<Base> *Store::Interface::cloneImpl(void) 
  {
    ptr<Statement<Store> >::type stmt(Statement<Store>::make(
                                             getLeftExpression()->clone(),
                                             getRightExpression()->clone()));
    Statement<Store> *result = stmt.get();
    stmt.reset();
    return result;
  }

  Store::Interface::Interface(ptr<Expression<Base> >::type e1,
                              ptr<Expression<Base> >::type e2)
      : Statement<DualExpression>(e1, e2), LeafStatement() {
    doValidation();
  }

  void Store::Interface::setParents(void)
  {
    getLeftExpression()->setParent(getSharedHandle());
    getRightExpression()->setParent(getSharedHandle());
  }

  void Store::Interface::doValidation(void) const {
    // Make sure e1 is of pointer type.
    if (!dyn_cast<const Symbol<Type<Pointer> > >(getLeftExpression()->type())) {
      std::cerr << "Offending statement:\n";
      print(std::cerr, this->getSharedHandle());
    }
    checkInvariant(dyn_cast<const Symbol<Type<Pointer> > >(getLeftExpression()->type()),
                   "Store target must have pointer type");
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
