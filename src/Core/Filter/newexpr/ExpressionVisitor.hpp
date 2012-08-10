#ifndef mirv_Core_Filter_ExpressionVisitor_hpp
#define mirv_Core_Filter_ExpressionVisitor_hpp

#include <mirv/Core/Filter/newexpr/Visitor.hpp>
#include <mirv/Core/IR/newexpr/Expression.hpp>

namespace mirv {
  class ExpressionVisitor : public Visitor<Expression> {
  public:
    typedef Expresion VisitKind;

    /// This is the default implementation for visiting Expressions.
    template<Expression::Kind Kind>
    void visit(void) {
      Visitor<Expression>::visit<Kind>();
    }
  };
}

#endif
