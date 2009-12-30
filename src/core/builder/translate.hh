#ifndef mirv_core_builder_translate_hh
#define mirv_core_builder_translate_hh

#include <mirv/core/ir/statement.hh>
#include <mirv/core/ir/expression.hh>

namespace mirv {
   namespace Builder {
     template<typename Expr>
     ptr<Expression<Base> >::type 
     translate_expression(const Expr &expr) {
     }

     template<typename Expr>
     ptr<Statement<Base> >::type 
     translate_statement(const Expr &expr) {
     }
   }
}

#endif
