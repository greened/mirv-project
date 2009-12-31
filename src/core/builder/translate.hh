#ifndef mirv_core_builder_translate_hh
#define mirv_core_builder_translate_hh

#include <mirv/core/ir/statement.hh>
#include <mirv/core/ir/expression.hh>
#include <mirv/core/builder/grammar.hh>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     template<typename Expr>
     ptr<Expression<Base> >::type
     translate_expression(const Expr &expr) {
       ConstructExpressionGrammar translate;
       return translate(expr);
     }
#if 0
     template<typename Expr>
     ptr<Statement<Base> >::type 
     translate_statement(const Expr &expr) {
     }
#endif
   }
}

#endif
