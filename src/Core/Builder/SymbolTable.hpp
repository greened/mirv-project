#ifndef mirv_Core_Builder_SymbolTable_hpp
#define mirv_Core_Builder_SymbolTable_hpp

#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/proto/proto.hpp>

#include <list>
#include <map>
#include <sstream>
#include <stack>

namespace mirv {
  namespace Builder {
    /// This is a proto object transform to hold references to the
    /// created module and function symbols.  This allows children to
    /// query for various symbols as needed.
    class SymbolTable {
    private:
      typedef ptr<Module> ModulePointer;
      ModulePointer module;
      typedef ptr<Function> FunctionPointer;
      FunctionPointer function;
      typedef ptr<Control> StatementPointer;
      typedef std::list<StatementPointer> StatementList;
      StatementList pendingStatements;

      typedef std::map<
        std::string,
        ptr<const PlaceholderType>
        > PlaceholderMap;
      PlaceholderMap placeholders;

      typedef std::map<
        std::string,
        std::string
        > PlaceholderNameMap;
      PlaceholderNameMap PlaceholderNames;

      class Scope {
        ptr<Block> TheBlock;

        using AllocateMap = std::map<std::string, ptr<Allocate>>;
        AllocateMap Allocates;

      public:
        Scope(void) : TheBlock(IRBuilder::get<Block>()) {}

        ~Scope(void) {
          if (TheBlock) {
            TheBlock.destroy();
          }
        }

        [[nodiscard]]
        ptr<Allocate> lookupAllocate(std::string Name) {
          auto I = Allocates.find(Name);
          if (I != Allocates.end()) {
            return I->second;
          }
          error("Could not find allocate");
          return nullptr;
        }

        [[nodiscard]]
        ptr<Block> getBlock(void) {
          return TheBlock;
        }

        [[nodiscard]]
        ptr<Block> claimBlock(void) {
          auto B = getBlock();
          TheBlock.reset();
          return B;
        }
      };

      std::stack<Scope> Stack;

      std::string translatePlaceholderName(const std::string &name) const;

    public:
      SymbolTable(ModulePointer m, FunctionPointer f);

      static ptr<SymbolTable> make(ModulePointer m);

      void setModule(ModulePointer m);
      //void clearModule(void);
      ModulePointer getModule(void) const;

      void setFunction(FunctionPointer f);
      //void clearFunction(void);
      FunctionPointer getFunction(void) const;

      void pushScope(void) {
        Stack.emplace();
      }

      void popScope(void) {
        checkInvariant(!Stack.empty(), "Scope underflow");
        Stack.pop();
      }

      Scope &getCurrentScope(void) {
        return Stack.top();
      }

      const Scope &getCurrentScope(void) const {
        return Stack.top();
      }

#if 0
      void addPendingStatement(StatementPointer s) {
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
#endif
#if 0
      ptr<const Type>
      addPlaceholder(const std::string &name);

      ptr<const PlaceholderType>
      lookupPlaceholder(const std::string &name) const;

      ptr<const PlaceholderType>
      removePlaceholder(const std::string &name);

      void resolve(const std::string &oldName,
                   ptr<const PlaceholderType> placeholder,
                   ptr<const Type> replacement);

      /// Get the variable symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<Allocate>
      lookupAtCurrentScope(const std::string &name, Allocate *) const;

      /// Get the global variable symbol at the current scope only.
      /// Return a null pointer if the symbol does not exist.
      ptr<GlobalVariable>
      lookupAtCurrentScope(const std::string &name, GlobalVariable *) const;

      /// Get the function symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<Function>
      lookupAtModuleScope(const std::string &name, Function *) const;
      ptr<Function>
      lookupAtCurrentScope(const std::string &name, Function *) const;

      /// Get the global variable symbol at module scope only.  Return
      /// a null pointer if the symbol does not exist.
      ptr<GlobalVariable>
      lookupAtModuleScope(const std::string &name, GlobalVariable *) const;

      /// Get the type symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<const Type>
      lookupAtModuleScope(const std::string &name, const Type *) const;

      ptr<Allocate>
      lookupAtAllScopes(const std::string &name, Allocate *) const;
      ptr<GlobalVariable>
      lookupAtAllScopes(const std::string &name, GlobalVariable *) const;
      ptr<Function>
      lookupAtAllScopes(const std::string &name, Function *) const;
      ptr<const Type>
      lookupAtAllScopes(const std::string &name, const Type *) const;
      void addAtCurrentScope(ptr<Allocate> var);
      void addAtCurrentScope(ptr<GlobalVariable> var);
      void addAtCurrentScope(ptr<Function> func);
      //void addAtCurrentScope(ptr<const Type> type);
      void addAtModuleScope(ptr<Function> func);
      //void addAtModuleScope(ptr<const Type> type);
      void addAtModuleScope(ptr<GlobalVariable> var);
#endif
    };

    /// This is a callable transform to set the module scope in a
    /// symbol table.
    struct SetModule : boost::proto::callable {
      typedef ptr<SymbolTable> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
			     ptr<Module> module) {
	symtab->setModule(module);
        return symtab;
      }
    };

    /// This is a callable transform to get the module scope from a
    /// symbol table.
    struct GetModule : boost::proto::callable {
      typedef ptr<Module> result_type;

      result_type operator()(ptr<SymbolTable> symtab) {
        return IRBuilder::getCurrentModule();
        //return symtab->getModule();
      }
    };

    /// This is a callable transform to set the function scope in a
    /// symbol table.
    struct SetFunction : boost::proto::callable {
      typedef ptr<SymbolTable> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
			     ptr<Function> function) {
        std::cerr << "SetFunction\n";
	symtab->setFunction(function);
        return symtab;
      }
    };

    /// This is a callable transform to push a new scope.
    struct PushScope : boost::proto::callable {
      typedef ptr<SymbolTable> result_type;

      result_type operator()(ptr<SymbolTable> symtab) {
        std::cerr << "PushScope\n";
        symtab->pushScope();
        return symtab;
      }
    };

    /// This is a callable transform to pop a scope.
    struct PopScope : boost::proto::callable {
      typedef ptr<Block> result_type;

      result_type operator()(ptr<Block> TheBlock,
                             ptr<SymbolTable> symtab) {
        std::cerr << "PopScope(Block)\n";
        auto B = symtab->getCurrentScope().claimBlock();

        if (B != TheBlock) {
          error("Did not claim given block");
        }

        symtab->popScope();

        return TheBlock;
      }
      result_type operator()(ptr<Control> Stmt,
                             ptr<SymbolTable> symtab) {
        std::cerr << "PopScope(Control)\n";
        auto B = symtab->getCurrentScope().claimBlock();

        symtab->popScope();

        if (auto TheBlock = dyn_cast<Block>(Stmt)) {
          if (B != TheBlock) {
            error("Did not claim given block");
          }
          return TheBlock;
        }

        // Single statement.  Put it in the block if not already
        // there.
        if (B->back() != Stmt) {
          B->push_back(Stmt);
        }

        return B;
      }
    };

    /// This is a callable transform to lookup a symbol.
    template<typename SymbolType,
      typename Dummy = boost::proto::callable>
    struct LookupSymbol : boost::proto::callable {
      typedef ptr<SymbolType> result_type;

      result_type operator()(ptr<const SymbolTable> symtab,
			     const std::string &name) {
        error("Use IRBuilder to look up symbols");
	// result_type result =
        //   symtab->lookupAtAllScopes(name, reinterpret_cast<SymbolType *>(0));
	// if (!result) {
	//   error("Symbol does not exist");
	// }
        // return result;
        return result_type();
      }
    };

    /// This is a grammar action to look up type symbols.  We
    /// speciailize it for types, which must be const.
    template<>
    struct LookupSymbol<Type, boost::proto::callable>
        : boost::proto::callable {
      typedef ptr<const Type> result_type;

      result_type operator()(ptr<const SymbolTable> symtab,
			     const std::string &name) {
	result_type result = IRBuilder::findTupleType(name);
        if (!result) {
          result = IRBuilder::findPlaceholderType(name);
        }
	if (!result) {
	  error("Symbol does not exist");
	}
        return result;
      }
    };

    /// This is a callable transform to lookup a symbol and add it to
    /// the current scope if it does not exist.
    template<typename SymbolType, typename Dummy = boost::proto::callable>
    struct LookupAndAddSymbol : boost::proto::callable {
      typedef ptr<SymbolType> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
			     result_type symbol) {
        error("Use IRBuilder to find symbols");
	// result_type result =
        //   symtab->lookupAtAllScopes(symbol->name(),
        //                             reinterpret_cast<SymbolType *>(0));
	// if (!result) {
	//   symtab->addAtCurrentScope(symbol);
        //   result = symbol;
	// }
	// else {
	//   symbol.reset();
	// }
        // return result;
        return result_type();
      }
    };

    /// This is a grammar action to look up a type symbol.  Specialize
    /// for types as they must be const.
    template<>
    struct LookupAndAddSymbol<Type, boost::proto::callable>
        : boost::proto::callable {
      typedef ptr<const Type> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
			     result_type symbol) {
        error("Use IRBuilder for types");
        // std::ostringstream name;
        // print(name, symbol);
	// result_type result =
        //   symtab->lookupAtModuleScope(name.str(), 
        //                               reinterpret_cast<T *>(0));
	// if (!result) {
	//   symtab->addAtCurrentScope(symbol);
        //   result = symbol;
	// }
	// else {
	//   symbol.reset();
	// }
        // return result;
        return result_type();
      }
    };
  }
}

#endif
