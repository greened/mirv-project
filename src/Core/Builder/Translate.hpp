#ifndef mirv_Core_Builder_Translate_hpp
#define mirv_Core_Builder_Translate_hpp

#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>

#include <boost/proto/proto.hpp>
#include <boost/mpl/assert.hpp>

namespace mirv {
  class Function;
  class Module;

  namespace Builder {
    /// This is a utility to check if a builder fragment matches the
    /// builder grammar.  It is mainly used for debugging.
    template<typename Grammar, typename Expr>
    void
    checkMatch(const Expr &expr) {
      BOOST_MPL_ASSERT(( boost::proto::matches<typename boost::proto::result_of::as_expr<Expr, Domain>::type, Grammar> ));
    }

    template<typename Grammar, typename Expr>
    auto translateWithGrammar(const Expr &expr,
                              ptr<SymbolTable> symtab) {
      //std::cout << "Translating:\n";
      //boost::proto::display_expr(expr);

      checkMatch<Grammar>(expr);

      Grammar trans;
      symtab->pushScope();
      auto Result = trans(expr, 0, symtab);
      auto B = symtab->getCurrentScope().claimBlock();
      symtab->popScope();

      using ResultType = decltype(Result);
      using BType = decltype(B);

      if constexpr (std::is_same_v<typename ResultType::PointeeType,
                                   typename BType::PointeeType>) {
          if (Result != B) {
            B->clear();
            B.destroy();
          }
        }
      else {
        B->clear();
        B.destroy();
      }

      return Result;
    }

    /// This is the utility to transform a proto build expression to
    /// MIRV IR.  It allows the client to specify the grammar to use
    /// for translation.
    template<typename Grammar, typename Expr>
    auto translateWithGrammar(ptr<Module> module,
                              ptr<Function> function,
                              const Expr &expr) {
      ptr<SymbolTable> symtab(new SymbolTable(module, function));
      return translateWithGrammar<Grammar, Expr>(expr, symtab);
    }

    template<typename Grammar, typename Expr>
    auto translateWithGrammar(ptr<Module> module,
                              const Expr &expr) {
      //std::cout << "Translating:\n";
      //boost::proto::display_expr(expr);
      ptr<SymbolTable> symtab(new SymbolTable(module, ptr<Function>()));
      return translateWithGrammar<Grammar, Expr>(expr, symtab);
    }

    template<typename Grammar, typename Expr>
    auto translateWithGrammar(const Expr &expr) {
      //std::cout << "Translating:\n";
      //boost::proto::display_expr(expr);
      ptr<SymbolTable> symtab(new SymbolTable(ptr<Module>(), ptr<Function>()));
      return translateWithGrammar<Grammar, Expr>(expr, symtab);
    }
  }
}

#endif
