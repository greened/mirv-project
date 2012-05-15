#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/IR/Mutating.hpp>
#include <mirv/Core/IR/PointerType.hpp>
#include <mirv/Core/Utility/Printer.hpp>

namespace mirv {
  Statement<Base> *detail::PhiInterface::cloneImpl(void) 
  {
    ptr<Statement<Phi> > stmt(Statement<Phi>::make(this->target()));
    Statement<Phi>::iterator i = this->begin();
    while (++i != this->end()) {
      stmt->expressionPushBack((*i)->clone());
    }
    Statement<Phi> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::PhiInterface::setParents(void)
  {
    for (ExpressionIterator i = expressionBegin();
         i != expressionEnd();
         ++i) {
      (*i)->setParent(getSharedHandle());
    }
  }

  Statement<Base> *detail::StoreInterface::cloneImpl(void) 
  {
    ptr<Statement<Store> > stmt(Statement<Store>::make(
                                             getLeftExpression()->clone(),
                                             getRightExpression()->clone()));
    Statement<Store> *result = stmt.get();
    stmt.reset();
    return result;
  }

  detail::StoreInterface::StoreInterface(ptr<Expression<Base> > e1,
                                         ptr<Expression<Base> > e2)
      : Statement<DualExpression>(e1, e2), LeafStatement() {
    doValidation();
  }

  void detail::StoreInterface::setParents(void)
  {
    getLeftExpression()->setParent(getSharedHandle());
    getRightExpression()->setParent(getSharedHandle());
  }

  void detail::StoreInterface::doValidation(void) const {
    // Make sure e1 is of pointer type.
    if (!dyn_cast<const Symbol<Type<Pointer> > >(getLeftExpression()->type())) {
      std::cerr << "Offending statement:\n";
      print(std::cerr, this->getSharedHandle());
    }
    checkInvariant(dyn_cast<const Symbol<Type<Pointer> > >(getLeftExpression()->type()),
                   "Store target must have pointer type");
  }

  Statement<Base> *detail::CallInterface::cloneImpl(void) 
  {
    ptr<Statement<Call> > stmt(Statement<Call>::make(this->function()->clone()));
    Statement<Call>::iterator i = this->begin();
    while (++i != this->end()) {
      stmt->expressionPushBack((*i)->clone());
    }
    Statement<Call> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::CallInterface::setParents(void) 
  {
    for (ExpressionIterator i = expressionBegin();
         i != expressionEnd();
         ++i) {
      (*i)->setParent(getSharedHandle());
    }
  }

  Statement<Base> *detail::AllocateInterface::cloneImpl(void) 
  {
    ptr<Statement<Allocate> > stmt(Statement<Allocate>::make(
                                           getLeftExpression()->clone(),
                                           getRightExpression()->clone(),
                                           type()));
    Statement<Allocate> *result = stmt.get();
    stmt.reset();
    return result;
  }

  void detail::AllocateInterface::setParents(void)
  {
    getLeftExpression()->setParent(getSharedHandle());
    getRightExpression()->setParent(getSharedHandle());
  }
}
