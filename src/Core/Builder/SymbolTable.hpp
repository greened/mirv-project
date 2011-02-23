#ifndef mirv_Core_Builder_SymbolTable_hpp
#define mirv_Core_Builder_SymbolTable_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/VariableFwd.hpp>
#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/FunctionFwd.hpp>
#include <mirv/Core/IR/StatementFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/proto/proto.hpp>

#include <list>

namespace mirv {
  namespace Builder {
    /// This is a proto object transform to hold references to the
    /// created module and funceion symbols.  This allows children to
    /// query for various symbols as needed.
    class SymbolTable {
    private:
      typedef ptr<Symbol<Module> >::type ModulePointer;
      ModulePointer module;
      typedef ptr<Symbol<Function> >::type FunctionPointer;
      FunctionPointer function;
      typedef ptr<Statement<Base> >::type StatementPointer;
      typedef std::list<StatementPointer> StatementList;
      StatementList pendingStatements;
      unsigned int tempNum;

    public:
      SymbolTable(ModulePointer m, FunctionPointer f)
          : module(m), function(f), tempNum(0) {}

      static ptr<SymbolTable>::type make(ModulePointer m);

      unsigned int getNextTempNum(void) {
        return tempNum++;
      }

      void setModule(ModulePointer m) {
	module = m;
      }
      void clearModule(void) {
	module.reset();
      }
      ModulePointer getModule(void) const {
	return module;
      }

      void setFunction(FunctionPointer f) {
	function = f;
      }
      void clearFunction(void) {
	function.reset();
      }
      FunctionPointer getFunction(void) const {
	return function;
      }

      void addPendingStatment(StatementPointer s) {
        pendingStatements.push_back(s);
      }
      void clearPendingStatements(void) {
        pendingStatements.clear();
      }

      typedef StatementList::iterator StatementIterator;
      StatementIterator pendingStatementsBegin(void) {
        return pendingStatements.begin();
      }
      StatementIterator pendingStatementsEnd(void) {
        return pendingStatements.end();
      }

      bool pendingStatementsEmpty(void) const {
        return pendingStatements.empty();
      }

      /// Get the variable symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<Symbol<Variable> >::type
      lookupAtCurrentScope(const std::string &name,
			   Symbol<Variable> *) const;

      /// Get the function symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<Symbol<Function> >::type
      lookupAtCurrentScope(const std::string &name,
			   Symbol<Function> *) const;

      /// Get the type symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<Symbol<Type<TypeBase> > >::const_type
      lookupAtCurrentScope(const std::string &name,
			   const Symbol<Type<TypeBase> > *) const;

      ptr<Symbol<Variable> >::type
      lookupAtAllScopes(const std::string &name,
			Symbol<Variable> *) const;
      ptr<Symbol<Function> >::type
      lookupAtAllScopes(const std::string &name,
			Symbol<Function> *) const;
      ptr<Symbol<Type<TypeBase> > >::const_type
      lookupAtAllScopes(const std::string &name,
			const Symbol<Type<TypeBase> > *) const;
      void addAtCurrentScope(ptr<Symbol<Variable> >::type var);
      void addAtCurrentScope(ptr<Symbol<Function> >::type func);
      void addAtCurrentScope(ptr<Symbol<Type<TypeBase> > >::const_type type);
    };

    /// This is a callable transform to set the module scope in a
    /// symbol table.
    struct SetModule : boost::proto::callable {
      typedef ptr<SymbolTable>::type result_type;

      result_type operator()(ptr<SymbolTable>::type symtab,
			     ptr<Symbol<Module> >::type module) {
	symtab->setModule(module);
        return symtab;
      }
    };

    /// This is a callable transform to get the module scope from a
    /// symbol table.
    struct GetModule : boost::proto::callable {
      typedef ptr<Symbol<Module> >::type result_type;

      result_type operator()(ptr<SymbolTable>::type symtab) {
        return symtab->getModule();
      }
    };

    /// This is a callable transform to set the function scope in a
    /// symbol table.
    struct SetFunction : boost::proto::callable {
      typedef ptr<SymbolTable>::type result_type;

      result_type operator()(ptr<SymbolTable>::type symtab,
			     ptr<Symbol<Function> >::type function) {
	symtab->setFunction(function);
        return symtab;
      }
    };

    /// This is a callable transform to lookup a symbol.
    template<typename SymbolType,
      typename Dummy = boost::proto::callable>
    struct LookupSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      result_type operator()(ptr<SymbolTable>::const_type symtab,
			     const std::string &name) {
	result_type result =
          symtab->lookupAtAllScopes(name, reinterpret_cast<SymbolType *>(0));
	if (!result) {
	  error("Symbol does not exist");
	}
        return result;
      }
    };

    // Specialize for types which must be const.
    template<typename Tag>
    struct LookupSymbol<Symbol<Type<Tag> >, boost::proto::callable>
        : boost::proto::callable {
      typedef typename ptr<Symbol<Type<Tag> > >::const_type result_type;

      result_type operator()(ptr<SymbolTable>::const_type symtab,
			     const std::string &name) {
	result_type result =
          symtab->lookupAtAllScopes(name,
                                    reinterpret_cast<const Symbol<Type<Tag> > *>(0));
	if (!result) {
	  error("Symbol does not exist");
	}
        return result;
      }
    };

    /// This is a callable transform to lookup a symbol and add it to
    /// the current scope if it does not exist.
    template<typename SymbolType,
      typename Dummy = boost::proto::callable>
    struct LookupAndAddSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      result_type operator()(ptr<SymbolTable>::type symtab,
			     result_type symbol) {
	result_type result =
          symtab->lookupAtAllScopes(symbol->name(), 
                                    reinterpret_cast<SymbolType *>(0));
	if (!result) {
	  symtab->addAtCurrentScope(symbol);
          result = symbol;
	}
	else {
	  symbol.reset();
	}
        return result;
      }
    };

    // Specialize for types as they must be const.
    template<typename Tag>
    struct LookupAndAddSymbol<Symbol<Type<Tag> >, boost::proto::callable> 
        : boost::proto::callable {
      typedef typename ptr<Symbol<Type<Tag> > >::const_type result_type;

      result_type operator()(ptr<SymbolTable>::type symtab,
			     result_type symbol) {
	result_type result =
          symtab->lookupAtAllScopes(symbol->name(), 
                                    reinterpret_cast<Symbol<Type<Tag> > *>(0));
	if (!result) {
	  symtab->addAtCurrentScope(symbol);
          result = symbol;
	}
	else {
	  symbol.reset();
	}
        return result;
      }
    };
  }
}

#endif
