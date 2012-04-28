#ifndef mirv_Core_Builder_ConstructSymbolTransform_hpp
#define mirv_Core_Builder_ConstructSymbolTransform_hpp

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
    enum Scope {
      ModuleScope,
      CurrentScope
    };
    namespace detail {
      /// This is a functor to get the canonical name of a type.
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

      /// This is a helper functor to get the canonical name of an
      /// integer type.
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

      template<typename SymbolType, Scope scope>
      struct Lookup;

      /// This is a grammar action to look up a symbol in the corrent
      /// function.
      template<typename SymbolType> 
      struct Lookup<SymbolType, CurrentScope> {
        typedef typename ptr<SymbolType>::type result_type;

        template<typename ...Args>
        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               Args... a) {
          std::string name = SymbolType::getName(a...);
          return symtab->lookupAtCurrentScope(name, reinterpret_cast<SymbolType *>(0));
        }
      };

      template<typename SymbolType> 
      struct Lookup<SymbolType, ModuleScope> {
        typedef typename ptr<SymbolType>::type result_type;

        template<typename ...Args>
        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               Args... a) {
          std::string name = SymbolType::getName(a...);
          return symtab->lookupAtModuleScope(name, reinterpret_cast<SymbolType *>(0));
        }
      };

      template<Scope scope>
      struct Add;

      /// This is a grammar action to add a symbol to the current
      /// function or module.
      template<> 
      struct Add<CurrentScope> {
        template<typename SymbolType>
        void operator()(boost::shared_ptr<SymbolTable> symtab,
                        SymbolType a) {
          symtab->addAtCurrentScope(a);
        }
      };

      /// This is a grammar action to add a symbol to a module.
      template<> 
      struct Add<ModuleScope> {
        template<typename SymbolType>
        void operator()(boost::shared_ptr<SymbolTable> symtab,
                        SymbolType a) {
          symtab->addAtModuleScope(a);
        }
      };
    }

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<
      typename SymbolType,
      Scope scope,
      typename Dummy = boost::proto::callable
      >
    struct UnaryConstructSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      template<typename Arg>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg a) {
        std::string name = SymbolType::getName(a);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::type exists =
          detail::Lookup<SymbolType, scope>()(symtab, a);
	if (exists) {
	  error("Symbol exists");
	}
	result_type result = mirv::make<SymbolType>(a);

        detail::Add<scope>()(symtab, result);

	return result;
      }
    };

    /// This is a grammar action to construct Symbols with
    /// one-argument constructors.  Specialize for types, which need
    /// to be const.
    template<typename Tag>
    struct UnaryConstructSymbol<
      Symbol<Type<Tag> >,
      ModuleScope,
      boost::proto::callable
      >
        : boost::proto::callable {
      typedef typename ptr<Symbol<Type<TypeBase> > >::const_type result_type;

      template<typename Arg>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg a) {
        std::string name = detail::GetName<Tag>()(symtab->getModule(), a);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Type<TypeBase> > >::const_type exists =
          symtab->lookupAtModuleScope(name,
                                      reinterpret_cast<const Symbol<Type<Tag> > *>(0));
        if (exists) {
          // If this is a placeholder, we're about to replace it.
          typename ptr<Symbol<Type<Tag> > >::const_type type =
            dyn_cast<const Symbol<Type<Tag> > >(exists);
          if (type) {
            // It's ok to have a type already declared.
            return type;
          }
          symtab->removePlaceholder(name);
	}
        result_type result = mirv::make<Symbol<Type<Tag> > >(a);
        symtab->addAtModuleScope(result);
        return result;
      }
    };

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<
      typename SymbolType,
      Scope scope,
      typename Dummy = boost::proto::callable
      >
    struct BinaryConstructSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      template<typename Arg1, typename Arg2>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg1 a1,
			     Arg2 a2) {
	std::string name = SymbolType::getName(a1, a2);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::type exists =
          detail::Lookup<SymbolType, scope>()(symtab, a1, a2);
	if (exists) {
	  error("Symbol exists");
	}
	result_type result = mirv::make<SymbolType>(a1, a2);
        detail::Add<scope>()(symtab, result);

	return result;
      }
    };

    /// This is a grammar action to construct symbols with
    /// two-argument constructors. Specialize for types, which need to
    /// be const.
    template<typename Tag>
    struct BinaryConstructSymbol<
      Symbol<Type<Tag> >,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef typename ptr<Symbol<Type<TypeBase> > >::const_type result_type;

      template<typename Arg1, typename Arg2>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg1 a1,
			     Arg2 a2) {
        std::string name = detail::GetName<Tag>()(symtab->getModule(), a1, a2);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Type<TypeBase> > >::const_type exists =
          symtab->lookupAtModuleScope(name,
                                      reinterpret_cast<const Symbol<Type<Tag> >  *>(0));
	if (exists) {
          // If this is a placeholder, we're about to replace it.
          typename ptr<Symbol<Type<Tag> > >::const_type type =
            dyn_cast<const Symbol<Type<Tag> > >(exists);
          if (type) {
            // It's ok to have a type already declared.
            return type;
          }
          symtab->removePlaceholder(name);
	}
        result_type result = mirv::make<Symbol<Type<Tag> > >(a1, a2);
	symtab->addAtModuleScope(result);
	return result;
      }
    };

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<
      typename SymbolType,
      Scope scope,
      typename Dummy = boost::proto::callable
      >
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
          detail::Lookup<SymbolType, scope>()(symtab, a1, a2, a3);
	if (exists) {
	  error("Symbol exists");
	}
        result_type result = mirv::make<SymbolType>(a1, a2, a3);
        detail::Add<scope>()(symtab, result);

        return result;
      }
    };

    /// This is a grammar action to construct Symbols with
    /// three-argument constructors.  Specialize for types, which need
    /// to be const.
    template<typename Tag>
    struct TernaryConstructSymbol<
      Symbol<Type<Tag> >,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
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
          symtab->lookupAtModuleScope(name,
                                      reinterpret_cast<const Symbol<Type<Tag> >  *>(0));
	if (exists) {
          // If this is a placeholder, we're about to replace it.
          typename ptr<Symbol<Type<Tag> > >::const_type type =
            dyn_cast<const Symbol<Type<Tag> > >(exists);
          if (type) {
            // It's ok to have a type already declared.
            return type;
          }
          symtab->removePlaceholder(name);
	}
        result_type result = mirv::make<Symbol<Type<Tag> > >(a1, a2, a3);
	symtab->addAtModuleScope(result);
	return result;
      }
    };

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<
      typename SymbolType,
      Scope scope,
      typename Dummy = boost::proto::callable
      >
    struct QuaternaryConstructSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg1 a1,
			     Arg2 a2,
                             Arg3 a3,
                             Arg4 a4) {
        std::string name = SymbolType::getName(a1, a2, a3, a4);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::type exists =
          detail::Lookup<SymbolType, scope>()(symtab, a1, a2, a3, a4);
	if (exists) {
	  error("Symbol exists");
	}
        result_type result = mirv::make<SymbolType>(a1, a2, a3, a4);
        detail::Add<scope>()(symtab, result);

        return result;
      }
    };

    /// This is a grammar action to construct Symbols with
    /// three-argument constructors.  Specialize for types, which need
    /// to be const.
    template<typename Tag>
    struct QuaternaryConstructSymbol<
      Symbol<Type<Tag> >,
      ModuleScope,
      boost::proto::callable
      > : boost::proto::callable {
      typedef typename ptr<Symbol<Type<Tag> > >::const_type result_type;

      template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg1 a1,
			     Arg2 a2,
                             Arg3 a3,
                             Arg4 a4) {
        std::string name = detail::GetName<Tag>()(symtab->getModule(),
                                                  a1,
                                                  a2,
                                                  a3,
                                                  a4);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::const_type exists =
          symtab->lookupAtModuleScope(name,
                                      reinterpret_cast<const Symbol<Type<Tag> >  *>(0));
	if (exists) {
          // If this is a placeholder, we're about to replace it.
          typename ptr<Symbol<Type<Tag> > >::const_type type =
            dyn_cast<const Symbol<Type<Tag> > >(exists);
          if (type) {
            // It's ok to have a type already declared.
            return type;
          }
          symtab->removePlaceholder(name);
	}
        result_type result = mirv::make<Symbol<Type<Tag> > >(a1, a2, a3, a4);
	symtab->addAtModuleScope(result);
	return result;
      }
    };

    /// Given a GlobalVariable Symbol, construct an expression to
    /// reference its value.
    struct ConstructGlobalReference : boost::proto::callable {
      typedef ptr<Expression<Base> >::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             ptr<Symbol<GlobalVariable> >::type global);
    };
  }
}

#endif
