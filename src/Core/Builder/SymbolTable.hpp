#ifndef mirv_Core_Builder_SymbolTable_hpp
#define mirv_Core_Builder_SymbolTable_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/IR/Base.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/VariableFwd.hpp>
#include <mirv/Core/IR/GlobalVariableFwd.hpp>
#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/FunctionFwd.hpp>
#include <mirv/Core/IR/StatementFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>
#include <mirv/Core/IR/PlaceholderTypeFwd.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/proto/proto.hpp>

#include <list>
#include <map>
#include <sstream>

namespace mirv {
  namespace Builder {
    /// This is a proto object transform to hold references to the
    /// created module and funceion symbols.  This allows children to
    /// query for various symbols as needed.
    class SymbolTable {
    private:
      typedef ptr<Symbol<Module> > ModulePointer;
      ModulePointer module;
      typedef ptr<Symbol<Function> > FunctionPointer;
      FunctionPointer function;
      typedef ptr<Statement<Base> > StatementPointer;
      typedef std::list<StatementPointer> StatementList;
      StatementList pendingStatements;

      typedef std::map<
        std::string,
        ptr<const Symbol<Type<Placeholder> > >
        > PlaceholderMap;
      PlaceholderMap placeholders;

      typedef std::map<
        std::string,
        std::string
        > NameMap;
      NameMap names;

      unsigned int tempNum;

      std::string translateName(const std::string &name) const;

    public:
      SymbolTable(ModulePointer m, FunctionPointer f);

      static ptr<SymbolTable> make(ModulePointer m);

      unsigned int getNextTempNum(void) {
        return tempNum++;
      }

      void setModule(ModulePointer m);
      void clearModule(void);
      ModulePointer getModule(void) const;

      void setFunction(FunctionPointer f);
      void clearFunction(void);
      FunctionPointer getFunction(void) const;

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

      ptr<const Symbol<Type<TypeBase> > >
      addPlaceholder(const std::string &name);

      ptr<const Symbol<Type<Placeholder> > >
      lookupPlaceholder(const std::string &name) const;

      ptr<const Symbol<Type<Placeholder> > >
      removePlaceholder(const std::string &name);

      void resolve(const std::string &oldName,
                   ptr<const Symbol<Type<Placeholder> > > placeholder,
                   ptr<const Symbol<Type<TypeBase> > > replacement);

      /// Get the variable symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<Symbol<Variable> >
      lookupAtCurrentScope(const std::string &name,
			   Symbol<Variable> *) const;

      /// Get the global variable symbol at the current scope only.
      /// Return a null pointer if the symbol does not exist.
      ptr<Symbol<GlobalVariable> >
      lookupAtCurrentScope(const std::string &name,
			   Symbol<GlobalVariable> *) const;

      /// Get the function symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<Symbol<Function> >
      lookupAtModuleScope(const std::string &name,
                          Symbol<Function> *) const;
      ptr<Symbol<Function> >
      lookupAtCurrentScope(const std::string &name,
                           Symbol<Function> *) const;

      /// Get the global variable symbol at module scope only.  Return
      /// a null pointer if the symbol does not exist.
      ptr<Symbol<GlobalVariable> >
      lookupAtModuleScope(const std::string &name,
                          Symbol<GlobalVariable> *) const;

      /// Get the type symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<const Symbol<Type<TypeBase> > >
      lookupAtModuleScope(const std::string &name,
                          const Symbol<Type<TypeBase> > *) const;

      ptr<Symbol<Variable> >
      lookupAtAllScopes(const std::string &name,
			Symbol<Variable> *) const;
      ptr<Symbol<GlobalVariable> >
      lookupAtAllScopes(const std::string &name,
			Symbol<GlobalVariable> *) const;
      ptr<Symbol<Function> >
      lookupAtAllScopes(const std::string &name,
			Symbol<Function> *) const;
      ptr<const Symbol<Type<TypeBase> > >
      lookupAtAllScopes(const std::string &name,
			const Symbol<Type<TypeBase> > *) const;
      void addAtCurrentScope(ptr<Symbol<Variable> > var);
      void addAtCurrentScope(ptr<Symbol<GlobalVariable> > var);
      void addAtCurrentScope(ptr<Symbol<Function> > func);
      void addAtCurrentScope(ptr<const Symbol<Type<TypeBase> > > type);
      void addAtModuleScope(ptr<Symbol<Function> > func);
      void addAtModuleScope(ptr<const Symbol<Type<TypeBase> > > type);
      void addAtModuleScope(ptr<Symbol<GlobalVariable> > var);
    };

    /// This is a callable transform to set the module scope in a
    /// symbol table.
    struct SetModule : boost::proto::callable {
      typedef ptr<SymbolTable> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
			     ptr<Symbol<Module> > module) {
	symtab->setModule(module);
        return symtab;
      }
    };

    /// This is a callable transform to get the module scope from a
    /// symbol table.
    struct GetModule : boost::proto::callable {
      typedef ptr<Symbol<Module> > result_type;

      result_type operator()(ptr<SymbolTable> symtab) {
        return symtab->getModule();
      }
    };

    /// This is a callable transform to set the function scope in a
    /// symbol table.
    struct SetFunction : boost::proto::callable {
      typedef ptr<SymbolTable> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
			     ptr<Symbol<Function> > function) {
	symtab->setFunction(function);
        return symtab;
      }
    };

    /// This is a callable transform to lookup a symbol.
    template<typename SymbolType,
      typename Dummy = boost::proto::callable>
    struct LookupSymbol : boost::proto::callable {
      typedef ptr<SymbolType> result_type;

      result_type operator()(ptr<const SymbolTable> symtab,
			     const std::string &name) {
	result_type result =
          symtab->lookupAtAllScopes(name, reinterpret_cast<SymbolType *>(0));
	if (!result) {
	  error("Symbol does not exist");
	}
        return result;
      }
    };

    /// This is a grammar action to look up type symbols.  We
    /// speciailize it for types, which must be const.
    template<typename Tag>
    struct LookupSymbol<Symbol<Type<Tag> >, boost::proto::callable>
        : boost::proto::callable {
      typedef ptr<const Symbol<Type<Tag> > > result_type;

      result_type operator()(ptr<const SymbolTable> symtab,
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
      typedef ptr<SymbolType> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
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

    /// This is a grammar action to look up a type symbol.  Specialize
    /// for types as they must be const.
    template<typename Tag>
    struct LookupAndAddSymbol<Symbol<Type<Tag> >, boost::proto::callable> 
        : boost::proto::callable {
      typedef ptr<const Symbol<Type<Tag> > > result_type;

      result_type operator()(ptr<SymbolTable> symtab,
			     result_type symbol) {
        std::ostringstream name;
        print(name, symbol);
	result_type result =
          symtab->lookupAtModuleScope(name.str(), 
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
