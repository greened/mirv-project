#ifndef mirv_Core_Builder_SymbolTransforms_hpp
#define mirv_Core_Builder_SymbolTransforms_hpp

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/SymbolGrammarFwd.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <boost/proto/proto.hpp>
#include <boost/mpl/print.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/type_traits.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to translate a proto expression
    /// to a symbol.
    template<typename SymbolType>
    class TranslateToSymbol : boost::proto::callable {
    private:
      ptr<SymbolTable> symtab;

    public:
      TranslateToSymbol<SymbolType>(ptr<SymbolTable> s)
      : symtab(s) {}

     typedef ptr<SymbolType> result_type;

      template<typename Expr>
      result_type operator()(const Expr &e) const {
        //std::cout << "Translating:\n";
        //boost::proto::display_expr(e);
        return safe_cast<SymbolType>(
          translateWithGrammar<ConstructSymbolGrammar>(e, symtab));
      }
    };

    /// This is a grammar action to translate a proto expression to an
    /// IR symbol.  This is a specialization for types, which must be
    /// const.
    template<typename T>
    class TranslateToSymbol<const T> : boost::proto::callable {
    private:
      ptr<SymbolTable> symtab;

    public:
      TranslateToSymbol<const T>(ptr<SymbolTable> s) :
        symtab(s) {}

      typedef ptr<T> result_type;

      template<typename Expr>
      auto operator()(const Expr &e) const {
        //std::cout << "Translating:\n";
        //boost::proto::display_expr(e);
        return translateWithGrammar<ConstructSymbolGrammar>(e, symtab);
      }
    };
  }
}

#endif
