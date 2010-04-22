#ifndef mirv_Core_Builder_Translate_hpp
#define mirv_Core_Builder_Translate_hpp

#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/Expression.hpp>
#include <mirv/Core/Builder/Grammar.hpp>

#include <boost/proto/proto.hpp>
#include <boost/mpl/assert.hpp>

namespace mirv {
   namespace Builder {
     /// This is a utility to check if a builder fragment matches the
     /// builder grammar.  It is mainly used for debugging.
     template<typename Grammar, typename Expr>
     void
     checkMatch(const Expr &expr) {
       BOOST_MPL_ASSERT(( boost::proto::matches<typename boost::proto::result_of::as_expr<Expr, Domain>::type, Grammar> ));
     }

     /// This is the utility to transform a proto build expression to
     /// a MIRV IR expression.
     template<typename Expr>
     ptr<Expression<Base> >::type
     translate_expression(ptr<Symbol<Module> >::type module,
			  ptr<Symbol<Function> >::type function,
			  const Expr &expr) {
       checkMatch<ConstructExpressionGrammar>(expr);
       ptr<SymbolTable>::type symtab(new SymbolTable(module, function));
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
       checkMatch<ConstructStatementGrammar>(expr);
       ptr<SymbolTable>::type symtab(new SymbolTable(module, function));
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
       checkMatch<ConstructGrammar>(expr);
       ptr<SymbolTable>::type symtab(new SymbolTable(module, function));
       ConstructGrammar trans;
       return trans(expr, 0, symtab);
     }

     template<typename Expr>
     ptr<Node<Base> >::type
     translate(ptr<Symbol<Module> >::type module,
	       const Expr &expr) {
       checkMatch<ConstructGrammar>(expr);
       ptr<SymbolTable>::type symtab(
         new SymbolTable(module, ptr<Symbol<Function> >::type()));
       ConstructGrammar trans;
       return trans(expr, 0, symtab);
     }

     template<typename Expr>
     ptr<Node<Base> >::type
     translate(const Expr &expr, ptr<SymbolTable>::type symtab) {
       checkMatch<ConstructGrammar>(expr);
       ConstructGrammar trans;
       return trans(expr, 0, symtab);
     }

     template<typename Expr>
     ptr<Node<Base> >::type
     translate(const Expr &expr) {
       checkMatch<ConstructGrammar>(expr);
       ptr<SymbolTable>::type symtab(
         new SymbolTable(ptr<Symbol<Module> >::type(),
                         ptr<Symbol<Function> >::type()));
       ConstructGrammar trans;
       return trans(expr, 0, symtab);
     }
   }
}

#endif
