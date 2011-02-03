#ifndef mirv_Core_Builder_TranslateFwd_hpp
#define mirv_Core_Builder_TranslateFwd_hpp

#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/FunctionFwd.hpp>
#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/StatementFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/Builder/TransformFwd.hpp>
#include <mirv/Core/Memory/Heap.hpp>

namespace mirv {
   namespace Builder {
     template<typename Grammar, typename Expr>
     void
     checkMatch(const Expr &expr);

     template<typename Expr>
     ptr<Expression<Base> >::type
     translate_expression(ptr<Symbol<Module> >::type module,
			  ptr<Symbol<Function> >::type function,
			  const Expr &expr);
     template<typename Expr>
     ptr<Statement<Base> >::type 
     translate_statement(ptr<Symbol<Module> >::type module,
			 ptr<Symbol<Function> >::type function,
			 const Expr &expr);

     template<typename Grammar, typename Expr>
     ptr<Node<Base> >::type
     translateWithGrammar(ptr<Symbol<Module> >::type module,
                          ptr<Symbol<Function> >::type function,
                          const Expr &expr);

     template<typename Grammar, typename Expr>
     ptr<Node<Base> >::type
     translateWithGrammar(ptr<Symbol<Module> >::type module,
                          const Expr &expr);

     template<typename Grammar, typename Expr>
     ptr<Node<Base> >::type
     translateWithGrammar(const Expr &expr, ptr<SymbolTable>::type symtab);

     template<typename Grammar, typename Expr>
     ptr<Node<Base> >::type
     translateWithGrammar(const Expr &expr);
   }
}

#endif
