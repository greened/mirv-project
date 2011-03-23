#ifndef mirv_Core_Builder_SymbolTransforms_hpp
#define mirv_Core_Builder_SymbolTransforms_hpp

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/SymbolGrammarFwd.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/IR/Base.hpp>
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
    namespace detail {
      template<typename Tag>
      struct GetName {
        template<typename ...Args>
        std::string operator()(ptr<Symbol<Module> >::type module,
                               Args ...args) {
          typename ptr<Symbol<Type<Tag> > >::const_type type =
            mirv::make<Symbol<Type<Tag> > >(args...);
          type->setParent(module);
          std::ostringstream name;
          print(name, type);
          return name.str();
        }
      };

      template<>
      struct GetName<Integral> {
        std::string operator()(ptr<Symbol<Module> >::type module,
                               std::uint64_t size) {
          return "int" + boost::lexical_cast<std::string>(size);
        }
      };

      template<>
      struct GetName<Floating> {
        std::string operator()(ptr<Symbol<Module> >::type module,
                               std::uint64_t size) {
          return "float" + boost::lexical_cast<std::string>(size);
        }
      };
    }

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<typename SymbolType,
	     typename Dummy = boost::proto::callable>
    struct UnaryConstructSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      template<typename Arg>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg a) {
        std::string name = SymbolType::getName(a);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::type exists =
          symtab->lookupAtCurrentScope(name,
                                       reinterpret_cast<SymbolType *>(0));
	if (exists) {
	  error("Symbol exists");
	}
	result_type result = mirv::make<SymbolType>(a);
	symtab->addAtCurrentScope(result);
	return result;
      }
    };

    // Specialize for types, which need to be const.
    template<typename Tag>
    struct UnaryConstructSymbol<Symbol<Type<Tag> >, boost::proto::callable>
        : boost::proto::callable {
      typedef typename ptr<Symbol<Type<TypeBase> > >::const_type result_type;

      template<typename Arg>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg a) {
        std::string name = detail::GetName<Tag>()(symtab->getModule(), a);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Type<TypeBase> > >::const_type exists =
          symtab->lookupAtCurrentScope(name,
                                       reinterpret_cast<const Symbol<Type<Tag> > *>(0));
        if (exists) {
          // If this is a placeholder, we're about to replace it.
          typename ptr<Symbol<Type<Tag> > >::const_type type =
            dyn_cast<const Symbol<Type<Tag> > >(exists);
          if (type) {
            // It's ok to have a type already declared.
            return type;
          }
          checkInvariant(safe_cast<const Symbol<Type<Placeholder> > >(exists),
                         "Unexpected type!");          
          symtab->removePlaceholder(name);
	}
        result_type result = mirv::make<Symbol<Type<Tag> > >(a);
        result->setParent(symtab->getModule());
        symtab->addAtCurrentScope(result);
        return result;
      }
    };

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<typename SymbolType,
	     typename Dummy = boost::proto::callable>
    struct BinaryConstructSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      template<typename Arg1, typename Arg2>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg1 a1,
			     Arg2 a2) {
	std::string name = SymbolType::getName(a1, a2);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::type exists =
          symtab->lookupAtCurrentScope(name, reinterpret_cast<SymbolType *>(0));
	if (exists) {
	  error("Symbol exists");
	}
	result_type result = mirv::make<SymbolType>(a1, a2);
	symtab->addAtCurrentScope(result);
	return result;
      }
    };

    // Specialize for types, which need to be const.
    template<typename Tag>
    struct BinaryConstructSymbol<Symbol<Type<Tag> >, boost::proto::callable>
        : boost::proto::callable {
      typedef typename ptr<Symbol<Type<TypeBase> > >::const_type result_type;

      template<typename Arg1, typename Arg2>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg1 a1,
			     Arg2 a2) {
        std::string name = detail::GetName<Tag>()(symtab->getModule(), a1, a2);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Type<TypeBase> > >::const_type exists =
          symtab->lookupAtCurrentScope(name,
                                       reinterpret_cast<const Symbol<Type<Tag> >  *>(0));
	if (exists) {
          // If this is a placeholder, we're about to replace it.
          typename ptr<Symbol<Type<Tag> > >::const_type type =
            dyn_cast<const Symbol<Type<Tag> > >(exists);
          if (type) {
            // It's ok to have a type already declared.
            return type;
          }
          checkInvariant(safe_cast<const Symbol<Type<Placeholder> > >(exists),
                         "Unexpected type!");          
          symtab->removePlaceholder(name);
	}
        result_type result = mirv::make<Symbol<Type<Tag> > >(a1, a2);
        result->setParent(symtab->getModule());
	symtab->addAtCurrentScope(result);
	return result;
      }
    };

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<typename SymbolType,
	     typename Dummy = boost::proto::callable>
    struct TernaryConstructSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      template<typename Arg1, typename Arg2, typename Arg3>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg1 a1,
			     Arg2 a2,
                             Arg3 a3) {
	std::string name = SymbolType::getName(a1, a2, a3);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::type exists =
          symtab->lookupAtCurrentScope(name, reinterpret_cast<SymbolType *>(0));
	if (exists) {
	  error("Symbol exists");
	}
        result_type result = mirv::make<SymbolType>(a1, a2, a3);
        symtab->addAtCurrentScope(result);
        return result;
      }
    };

    // Specialize for types which need to be const.
    template<typename Tag>
    struct TernaryConstructSymbol<Symbol<Type<Tag> >, boost::proto::callable>
        : boost::proto::callable {
      typedef typename ptr<Symbol<Type<Tag> > >::const_type result_type;

      template<typename Arg1, typename Arg2, typename Arg3>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg1 a1,
			     Arg2 a2,
                             Arg3 a3) {
        std::string name = detail::GetName<Tag>()(symtab->getModule(),
                                                  a1,
                                                  a2,
                                                  a3);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::const_type exists =
          symtab->lookupAtCurrentScope(name,
                                       reinterpret_cast<const Symbol<Type<Tag> >  *>(0));
	if (exists) {
          // If this is a placeholder, we're about to replace it.
          typename ptr<Symbol<Type<Tag> > >::const_type type =
            dyn_cast<const Symbol<Type<Tag> > >(exists);
          if (type) {
            // It's ok to have a type already declared.
            return type;
          }
          checkInvariant(safe_cast<const Symbol<Type<Placeholder> > >(exists),
                         "Unexpected type!");          
          symtab->removePlaceholder(name);
	}
        result_type result = mirv::make<Symbol<Type<Tag> > >(a1, a2, a3);
        result->setParent(symtab->getModule());
	symtab->addAtCurrentScope(result);
	return result;
      }
    };

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

    // Specialize for types which must be const.
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
