#ifndef mirv_Core_Builder_Translate_hpp
#define mirv_Core_Builder_Translate_hpp

#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/Builder/Grammar.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the utility to transform a proto build expression to
     /// a MIRV IR expression.
     template<typename Expr>
     ptr<Expression<Base> >::type
     translate_expression(const Expr &expr) {
       ConstructExpressionGrammar trans;
       return trans(expr);
     }

     /// This is the utility to transform a proto build expression to
     /// a MIRV IR statement.
     template<typename Expr>
     ptr<Statement<Base> >::type 
     translate_statement(const Expr &expr) {
       ConstructStatementGrammar trans;
       return trans(expr);
     }

     /// This is the utility to transform a proto build expression to
     /// MIRV IR.
     template<typename Expr>
     ptr<Node<Base> >::type
     translate(const Expr &expr) {
       ConstructGrammar trans;
       return trans(expr);
     }
   }
}

#endif
