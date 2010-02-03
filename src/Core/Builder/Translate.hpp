#ifndef mirv_Core_Builder_Translate_hpp
#define mirv_Core_Builder_Translate_hpp

#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/Builder/Grammar.hpp>

#include <boost/proto/proto.hpp>
#include <boost/mpl/assert.hpp>

namespace mirv {
   namespace Builder {
     /// This is the utility to transform a proto build expression to
     /// a MIRV IR expression.
     template<typename Expr>
     ptr<Expression<Base> >::type
     translate_expression(ptr<Symbol<Module> >::type module,
			  ptr<Symbol<Function> >::type function,
			  const Expr &expr) {
       BOOST_MPL_ASSERT(( boost::proto::matches<Expr, ConstructExpressionGrammar> ));
       SymbolTable symtab(module, function);
       ConstructExpressionGrammar trans;
       return trans(expr, 0, symtab);
     }

     /// This is the utility to transform a proto build expression to
     /// a MIRV IR statement.
     template<typename Expr>
     ptr<Statement<Base> >::type 
     translate_statement(ptr<Symbol<Module> >::type module,
			 ptr<Symbol<Function> >::type function,
			 const Expr &expr) {
       BOOST_MPL_ASSERT(( boost::proto::matches<Expr, ConstructStatementGrammar> ));
       SymbolTable symtab(module, function);
       ConstructStatementGrammar trans;
       return trans(expr, 0, symtab);
     }

     /// This is the utility to transform a proto build expression to
     /// MIRV IR.
     template<typename Expr>
     ptr<Node<Base> >::type
     translate(ptr<Symbol<Module> >::type module,
	       ptr<Symbol<Function> >::type function,
	       const Expr &expr) {
       BOOST_MPL_ASSERT(( boost::proto::matches<Expr, ConstructGrammar> ));
       SymbolTable symtab(module, function);
       ConstructGrammar trans;
       return trans(expr, 0, symtab);
     }
   }
}

#endif
