#ifndef mirv_Core_Builder_StatementTransforms_hpp
#define mirv_Core_Builder_StatementTransforms_hpp

#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/StatementFwd.hpp>
#include <mirv/Core/Builder/SymbolTableFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    struct StatementTransform : boost::proto::callable {
      typedef ptr<Statement<Base> > result_type;
    };

    struct IfTransform : public StatementTransform {
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<Expression<Base> > condition,
                             ptr<Statement<Base> > body);
    };

    struct IfElseTransform : public StatementTransform {
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<Expression<Base> > condition,
                             ptr<Statement<Base> > thenBody,
                             ptr<Statement<Base> > elseBody);
    };

    struct WhileTransform : public StatementTransform {
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<Expression<Base> > condition,
                             ptr<Statement<Base> > body);
    };

    struct DoWhileTransform : public StatementTransform {
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<Expression<Base> > condition,
                             ptr<Statement<Base> > body);
    };

    struct AssignTransform : public StatementTransform {
      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<Expression<Base> > lhs,
                             ptr<Expression<Base> > rhs);
    };
  }
}

#endif
