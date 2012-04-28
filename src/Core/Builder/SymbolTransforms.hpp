#ifndef mirv_Core_Builder_SymbolTransforms_hpp
#define mirv_Core_Builder_SymbolTransforms_hpp

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/SymbolGrammarFwd.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/FloatingTypeFwd.hpp>
#include <mirv/Core/IR/IntegralTypeFwd.hpp>
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
      boost::shared_ptr<SymbolTable> symtab;

    public:
      TranslateToSymbol<SymbolType>(boost::shared_ptr<SymbolTable> s)
      : symtab(s) {}

     typedef typename ptr<SymbolType>::type result_type;

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
    template<typename Tag>
    class TranslateToSymbol<Symbol<Type<Tag> > > : boost::proto::callable {
    private:
      boost::shared_ptr<SymbolTable> symtab;

    public:
      TranslateToSymbol<Symbol<Type<Tag> > >(boost::shared_ptr<SymbolTable> s)
      : symtab(s) {}

      typedef typename ptr<Symbol<Type<Tag> > >::const_type result_type;

      template<typename Expr>
      result_type operator()(const Expr &e) const {
        //std::cout << "Translating:\n";
        //boost::proto::display_expr(e);
        return safe_cast<const Symbol<Type<Tag> > >(
          constTranslateWithGrammar<ConstructSymbolGrammar>(e, symtab));
      }
    };
  }
}

#endif
