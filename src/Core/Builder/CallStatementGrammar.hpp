#ifndef mirv_Core_Builder_CallStatementGrammar_hpp
#define mirv_Core_Builder_CallStatementGrammar_hpp

#include <mirv/Core/Builder/ExpressionGrammar.hpp>

#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/CallStatementRules.hpp>
#include <mirv/Core/Builder/ConstructTransform.hpp>
#include <mirv/Core/IR/Function.hpp>

#include <boost/proto/proto.hpp>
#include <boost/fusion/include/pop_front.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to add a statement-level call to
    /// a function.
    struct AddSubroutineCallAsStatement : boost::proto::callable {
      typedef ptr<SubroutineCall> result_type;

      template<typename Arg, typename Expr>
      result_type operator()(ptr<SymbolTable> symtab,
                             Arg functionExpr,
                             const Expr &expr) {
        ptr<SubroutineCall> call;

        // If this function has a return type, we're ignoring it.
        call = ConstructNary<SubroutineCall>()(symtab,
                                               functionExpr,
                                               expr);
        return call;
      }
    };

    /// This is the grammar for function call statements.
    struct SubroutineCallStatementBuilder :  boost::proto::when<
      SubroutineCallRule,
      AddSubroutineCallAsStatement(
        boost::proto::_data,
        ConstructExpressionGrammar(boost::proto::_left),
        boost::proto::_expr)
      > {};
  }
}

#endif
