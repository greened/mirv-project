#ifndef mirv_Core_Builder_StatementTransforms_hpp
#define mirv_Core_Builder_StatementTransforms_hpp

#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/StatementFwd.hpp>
#include <mirv/Core/Builder/SymbolTableFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    struct StatementTransform : boost::proto::callable {
      typedef ptr<Statement<Base> >::type result_type;
    };

    struct IfTransform : public StatementTransform {
      result_type operator()(ptr<SymbolTable>::type symtab,
                             ptr<Expression<Base> >::type condition,
                             ptr<Statement<Base> >::type body);
    };

    struct IfElseTransform : public StatementTransform {
      result_type operator()(ptr<SymbolTable>::type symtab,
                             ptr<Expression<Base> >::type condition,
                             ptr<Statement<Base> >::type thenBody,
                             ptr<Statement<Base> >::type elseBody);
    };

    struct WhileTransform : public StatementTransform {
      result_type operator()(ptr<SymbolTable>::type symtab,
                             ptr<Expression<Base> >::type condition,
                             ptr<Statement<Base> >::type body);
    };

    struct DoWhileTransform : public StatementTransform {
      result_type operator()(ptr<SymbolTable>::type symtab,
                             ptr<Expression<Base> >::type condition,
                             ptr<Statement<Base> >::type body);
    };

    struct AssignTransform : public StatementTransform {
      result_type operator()(ptr<SymbolTable>::type symtab,
                             ptr<Expression<Base> >::type lhs,
                             ptr<Expression<Base> >::type rhs);
    };
  }
}

#endif
