#ifndef mirv_Core_Builder_Translate_hpp
#define mirv_Core_Builder_Translate_hpp

#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/FunctionFwd.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>

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
     /// MIRV IR.  It allows the client to specify the grammar to use
     /// for translation.
     template<typename Grammar, typename Expr>
     ptr<Node<Base> >
     translateWithGrammar(ptr<Symbol<Module> > module,
                          ptr<Symbol<Function> > function,
                          const Expr &expr) {
       //std::cout << "Translating:\n";
       //boost::proto::display_expr(expr);
       checkMatch<Grammar>(expr);
       ptr<SymbolTable> symtab(new SymbolTable(module, function));
       Grammar trans;
       return trans(expr, 0, symtab);
     }

     template<typename Grammar, typename Expr>
     ptr<Node<Base> >
     translateWithGrammar(ptr<Symbol<Module> > module,
                          const Expr &expr) {
       //std::cout << "Translating:\n";
       //boost::proto::display_expr(expr);
       checkMatch<Grammar>(expr);
       ptr<SymbolTable> symtab(
         new SymbolTable(module, ptr<Symbol<Function> >()));
       Grammar trans;
       return trans(expr, 0, symtab);
     }

     template<typename Grammar, typename Expr>
     ptr<Node<Base> >
     translateWithGrammar(const Expr &expr, ptr<SymbolTable> symtab) {
       //std::cout << "Translating:\n";
       //boost::proto::display_expr(expr);
       checkMatch<Grammar>(expr);
       Grammar trans;
       return trans(expr, 0, symtab);
     }

     template<typename Grammar, typename Expr>
     ptr<const Node<Base> >
     constTranslateWithGrammar(const Expr &expr, ptr<SymbolTable> symtab) {
       // std::cout << "Translating:\n";
       // boost::proto::display_expr(expr);
       checkMatch<Grammar>(expr);
       Grammar trans;
       return trans(expr, 0, symtab);
     }

     template<typename Grammar, typename Expr>
     ptr<Node<Base> >
     translateWithGrammar(const Expr &expr) {
       //std::cout << "Translating:\n";
       //boost::proto::display_expr(expr);
       checkMatch<Grammar>(expr);
       ptr<SymbolTable> symtab(
         new SymbolTable(ptr<Symbol<Module> >(),
                         ptr<Symbol<Function> >()));
       Grammar trans;
       return trans(expr, 0, symtab);
     }
   }
}

#endif
