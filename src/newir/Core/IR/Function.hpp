#ifndef mirv_Core_IR_Function_hpp
#define mirv_Core_IR_Function_hpp

#include <Core/IR/Statement.hpp>
#include <Core/IR/Expression.hpp>

#include <Core/IR/detail/IRNode.hpp>

namespace mirv {
  // Represent a function or routine.  We use multiple inheritance
  // instead of storing all the child indices in one Node becauee we
  // don't know _a_priori_ how many Statements and Expressions each
  // Function will have.
  class Function : public Node<Statement>,
                   public Node<Expression> {
  public:
    enum {
      IRKind = detail::IRNode::Function;
    };

    // This is safe because all Ranges are just sequences of integers.
    using Node<Statement>::Range;
    using Node<Statement>::ConstRange;

    template<typename U>
    Range children(void) {
      static_assert(0);
    }

    template<typename U>
    ConstRange children(void) const {
      static_assert(0);
    }

  private:
    Index<Module> theParent;
  };

  template<>
  Function::Range children<Statement>(void) {
    return Node<Statement>::children();
  }

  template<>
  Function::ConstRange children<Statement>(void) const {
    return Node<Statement>::children();
  }

  template<>
  Function::Range children<Expression>(void) {
    return Node<Expression>::children();
  }

  template<>
  Function::ConstRange children<Expression>(void) const {
    return Node<Expression>::children();
  }
}

#endif
