#ifndef mirv_Core_Builder_SymbolTransforms_hpp
#define mirv_Core_Builder_SymbolTransforms_hpp

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

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<typename SymbolType,
	     typename Dummy = boost::proto::callable>
    struct UnaryConstructSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      template<typename Arg>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg a,
                             Scope scope) {
	std::string name = SymbolType::getName(a);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::type exists =
          scope == ModuleScope ?
          symtab->lookupAtGlobalScope(name, reinterpret_cast<SymbolType *>(0))
          : symtab->lookupAtCurrentScope(name, reinterpret_cast<SymbolType *>(0));
	if (exists) {
          if (boost::is_base_and_derived<Symbol<Type<TypeBase> >,
              SymbolType>::value) {
            // It's ok to have a type already declared.
            return safe_cast<SymbolType>(exists);
          }
	  error("Symbol exists");
	}
	result_type result = mirv::make<SymbolType>(a);

	if (scope == ModuleScope) {
          symtab->addAtModuleScope(result);
        }
        else {
          symtab->addAtModuleScope(result);
        }

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
			     Arg2 a2,
                             Scope scope) {
	std::string name = SymbolType::getName(a1, a2);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::type exists =
          scope == ModuleScope ?
          symtab->lookupAtModuleScope(name, reinterpret_cast<SymbolType *>(0))
          : symtab->lookupAtCurrentScope(name, reinterpret_cast<SymbolType *>(0));
	if (exists) {
          if (boost::is_base_and_derived<Symbol<Type<TypeBase> >,
              SymbolType>::value) {
            // It's ok to have a type already declared.
            return safe_cast<SymbolType>(exists);
          }
	  error("Symbol exists");
	}
	result_type result = mirv::make<SymbolType>(a1, a2);
        if (scope == ModuleScope) {
          symtab->addAtModuleScope(result);
        }
        else {
          symtab->addAtCurrentScope(result);
        }

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
                             Arg3 a3,
                             Scope scope) {
	std::string name = SymbolType::getName(a1, a2, a3);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::type exists =
          scope == ModuleScope ?
          symtab->lookupAtModuleScope(name, reinterpret_cast<SymbolType *>(0))
          : symtab->lookupAtCurrentScope(name, reinterpret_cast<SymbolType *>(0));
	if (exists) {
          if (boost::is_base_and_derived<Symbol<Type<TypeBase> >,
              SymbolType>::value) {
            // It's ok to have a type already declared.
            return safe_cast<SymbolType>(exists);
          }
	  error("Symbol exists");
	}
        result_type result = mirv::make<SymbolType>(a1, a2, a3);
        if (scope == ModuleScope) {
          symtab->addAtModuleScope(result);
        }
        else {
          symtab->addAtCurrentScope(result);
        }

        return result;
      }
    };

    /// This is a callable transform to construct a constant symbol.
    /// If the symbol exists at the current module, it is an error.
    template<
      typename ConstantTypeGenerator,
      typename Dummy = boost::proto::callable>
    struct ConstructConstantSymbol : boost::proto::callable {
      typedef typename ptr<Symbol<Constant<Base>>>::type result_type;

      template<typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     const Expr &expr) {
        ConstantTypeGenerator typeGen;

        // Constant type
        ptr<Symbol<Type<TypeBase> > >::type constantType = 
          LookupSymbol<Symbol<Type<TypeBase> > >()(
            symtab,
            typeGen(sizeof(typename boost::proto::result_of::value<Expr>::type) * 8));

        typedef typename boost::proto::result_of::value<Expr>::type BaseType;
        typedef Constant<BaseType> ConstantType;

        result_type result =
          mirv::make<Symbol<ConstantType>>(constantType, boost::proto::value(expr));

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
        return safe_cast<SymbolType>(translate(e, symtab));
      }
    };
  }
}

#endif
