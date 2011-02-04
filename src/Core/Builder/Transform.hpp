#ifndef mirv_Core_Builder_Transform_hpp
#define mirv_Core_Builder_Transform_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>
#include <mirv/Core/Builder/TranslateFwd.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Control.hpp>

#include <boost/proto/proto.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>

#include <iterator>
#include <algorithm>

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

      static ptr<SymbolTable>::type make(ModulePointer m) {
        ptr<SymbolTable>::type result(new SymbolTable(m, FunctionPointer()));
        return result;
      }

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
			   Symbol<Variable> *) const {
	if (function) {
	  Symbol<Function>::VariableIterator i = function->variableFind(name);
	  if (i != function->variableEnd()) {
	    return *i;
	  }
        }
	Symbol<Module>::VariableIterator i = module->variableFind(name);
	if (i != module->variableEnd()) {
	  return *i;
	}
        return ptr<Symbol<Variable> >::type();
      } 
     
      /// Get the function symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<Symbol<Function> >::type
      lookupAtCurrentScope(const std::string &name,
			   Symbol<Function> *) const {
	Symbol<Module>::FunctionIterator i = module->functionFind(name);
	if (i != module->functionEnd()) {
	  return *i;
	}
	return ptr<Symbol<Function> >::type();
      }

      /// Get the type symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<Symbol<Type<TypeBase> > >::const_type
      lookupAtCurrentScope(const std::string &name,
			   const Symbol<Type<TypeBase> > *) const {
	Symbol<Module>::ConstTypeIterator i = module->typeFind(name);
	if (i != module->typeEnd()) {
	  return *i;
	}
	return ptr<Symbol<Type<TypeBase> > >::const_type();
      }

      ptr<Symbol<Variable> >::type
      lookupAtAllScopes(const std::string &name,
			Symbol<Variable> *) const {
	ptr<Symbol<Variable> >::type var =
	  lookupAtCurrentScope(name, reinterpret_cast<Symbol<Variable> *>(0));
	if (function && !var) {
	  // Look up at module scope
	  SymbolTable ModuleScope(module, ptr<Symbol<Function> >::type());
	  var = ModuleScope.lookupAtCurrentScope(
            name,
            reinterpret_cast<Symbol<Variable> *>(0));
        }
	if (!var) {
	  error("Could not find variable");
	}
	return var;
      } 
     
      ptr<Symbol<Function> >::type
      lookupAtAllScopes(const std::string &name,
			Symbol<Function> *) const {
	ptr<Symbol<Function> >::type function =
	  lookupAtCurrentScope(name, reinterpret_cast<Symbol<Function> *>(0));
	if (!function) {
	  error("Could not find function");
	}
	return function;
      }

      ptr<Symbol<Type<TypeBase> > >::const_type
      lookupAtAllScopes(const std::string &name,
			const Symbol<Type<TypeBase> > *) const {
	ptr<Symbol<Type<TypeBase> > >::const_type type =
	  lookupAtCurrentScope(name,
                               reinterpret_cast<const Symbol<Type<TypeBase> > *>(0));
        if (!type) {
	  error("Could not find type");
	}
        return type;
      }

      void addAtCurrentScope(ptr<Symbol<Variable> >::type var) {
	ptr<Symbol<Variable> >::type result =
	  lookupAtCurrentScope(var->name(),
                               reinterpret_cast<Symbol<Variable> *>(0));
        if (result) {
	  error("Variable already exists");
	}
        if (function) {
	  function->variablePushBack(var);
          return;
        }
        module->variablePushBack(var);
      }

      void addAtCurrentScope(ptr<Symbol<Function> >::type func) {
	ptr<Symbol<Function> >::type result =
	  lookupAtCurrentScope(func->name(),
                               reinterpret_cast<Symbol<Function> *>(0));
        if (result) {
	  error("Function already exists");
	}
        module->functionPushBack(func);
      }

      void addAtCurrentScope(ptr<Symbol<Type<TypeBase> > >::const_type type) {
	ptr<Symbol<Type<TypeBase> > >::const_type result =
	  lookupAtCurrentScope(type->name(),
                               reinterpret_cast<const Symbol<Type<TypeBase> > *>(0));
        if (result) {
	  error("Type already exists");
	}
        module->typePushBack(type);
      }
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
	  symtab->addAtCurrentScope(result);
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
	  symtab->addAtCurrentScope(result);
	}
	else {
	  symbol.reset();
	}
        return result;
      }
    };

#if 0
    /// This is a callable transform to add a symbol at the current
    /// scope.  If the symbol already exists, it is an error.
    template<typename SymbolType,
      typename Dummy = boost::proto::callable>
    struct AddAtCurrentScope : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      result_type operator()(ptr<SymbolTable>::type symtab,
			     result_type symbol) {
	symtab->addAtCurrentScope(symbol);
        return symbol;
      }
    };
#endif

    // Bundle any pending statements created from child expressions
    // with this statement.
    template<typename StatementType, typename Dummy = boost::proto::callable>
    struct ClearPendingStatements : boost::proto::callable {
      typedef ptr<Statement<Base> >::type StatementPointer;
      typedef StatementPointer result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             StatementPointer stmt) {
        // Add the pending statements to the function.

        // TODO: Should use splice but it exposes the symtab statement
        // list.
        for (SymbolTable::StatementIterator s =
               symtab->pendingStatementsBegin();
             s != symtab->pendingStatementsEnd();
             ++s) {
          symtab->getFunction()->statementPushBack(*s);
        }
        
        symtab->clearPendingStatements();

        // This will get added by the caller.
        return stmt;
      }
    };

    // Loops need to be handled specially.
    template<>
    struct ClearPendingStatements<Statement<DoWhile>, boost::proto::callable>
        : boost::proto::callable {
      typedef ptr<Statement<DoWhile> >::type StatementPointer;
      typedef StatementPointer result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             StatementPointer stmt) {
        // Add the pending statements to the loop body.

        // TODO: Should use splice but it exposes the symtab statement
        // lst.
        for (SymbolTable::StatementIterator s =
               symtab->pendingStatementsBegin();
             s != symtab->pendingStatementsEnd();
             ++s) {
          safe_cast<Statement<Block> >(stmt->getChildStatement())->
            push_back(*s);
        }
        
        symtab->clearPendingStatements();

        // This will get added by the caller.
        return stmt;
      }
    };

    // Since there is no while statement, indicate a while by
    // specializing on the grammar rule.
    struct WhileRule;

    template<>
    struct ClearPendingStatements<WhileRule, boost::proto::callable>
        : boost::proto::callable {
      typedef ptr<Statement<IfThen> >::type StatementPointer;
      typedef StatementPointer result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             StatementPointer stmt) {
        for (SymbolTable::StatementIterator s =
               symtab->pendingStatementsBegin();
             s != symtab->pendingStatementsEnd();
             ++s) {
          // Add the pending statements before the top test.
          safe_cast<Statement<Block> >(stmt->parent<Statement<Base> >())->
            push_back(*s);

          // Add the pending statements to the loop body.
          safe_cast<Statement<Block> > (
            safe_cast<Statement<DoWhile> >(
              *(safe_cast<Statement<Block> >
                (
                  stmt->getChildStatement()
                )->begin()
               )
            )->
            getChildStatement())->push_back((*s)->clone());
      }

      symtab->clearPendingStatements();

      // This will get added by the caller.
      return stmt;
    }
    };

    /// Transform a one-operand node into a single-child IR node.
    template<typename NodeType,
      typename Child = typename NodeType::ChildPtr,
      typename Dummy = boost::proto::callable>
    struct ConstructUnary : boost::proto::callable {
      typedef typename ptr<NodeType>::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Child child) {
        return make<NodeType>(child);
      }
    };
  
    /// Transform a two-operand node to a two-child IR node.
    template<typename NodeType,
      typename Child1 = typename NodeType::ChildPtr,
      typename Child2 = typename NodeType::ChildPtr,
      typename Dummy = boost::proto::callable>
    struct ConstructBinary : boost::proto::callable {
      typedef typename ptr<NodeType>::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Child1 left, Child2 right) {
        return make<NodeType>(left, right);
      }
    };

    /// This is a specialization for block statements to add the
    /// child to a block if it already exists.
    template<>
    struct ConstructBinary<Statement<Block>,
      Statement<Block>::ChildPtr,
      Statement<Block>::ChildPtr,
      boost::proto::callable> : boost::proto::callable {
      typedef ptr<Statement<Block> >::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Statement<Block>::ChildPtr left,
                             Statement<Block>::ChildPtr right) {
        if (ptr<Statement<Block> >::type lb =
            dyn_cast<Statement<Block> >(left)) {
          if (ptr<Statement<Block> >::type rb =
              dyn_cast<Statement<Block> >(right)) {
            std::copy(rb->begin(), rb->end(), std::back_inserter(*lb));
          }
          lb->push_back(right);
          return lb;
        }
        else if (ptr<Statement<Block> >::type rb =
                 dyn_cast<Statement<Block> >(right)) {
          rb->push_front(left);
          return rb;
        }
        return make<Statement<Block> >(left, right);
      }
    };

    /// Transform a three-operand node to a three-child IR node.
    template<typename NodeType,
      typename Child1 = typename NodeType::ChildPtr,
      typename Child2 = typename NodeType::ChildPtr,
      typename Child3 = typename NodeType::ChildPtr,
      typename Dummy = boost::proto::callable>
    struct ConstructTernary : boost::proto::callable {
      typedef typename ptr<NodeType>::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Child1 child1, Child2 child2, Child3 child3) {
        return make<NodeType>(child1, child2, child3);
      }
    };
  
    /// This is a callable transform to translate a proto expression
    /// to a mirv expression.
    template<typename ExpressionType>
    class TranslateToExpression : boost::proto::callable {
    private:
      boost::shared_ptr<SymbolTable> symtab;

    public:
      TranslateToExpression<ExpressionType>(boost::shared_ptr<SymbolTable> s)
      : symtab(s) {}

      typedef typename ptr<ExpressionType>::type result_type;

      template<typename Expr>
      result_type operator()(const Expr &e) const {
        //std::cout << "Translating:\n";
        //boost::proto::display_expr(e);
        return safe_cast<ExpressionType>(
          translateWithGrammar<ConstructExpressionGrammar>(e, symtab));
      }
    };

    // Transform for an n-ary expression
    template<typename NodeType, typename Dummy = boost::proto::callable>
    struct ConstructNary : boost::proto::callable {
      typedef typename ptr<NodeType>::type result_type;
        
      template<typename Arg1, typename Arg2>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg1 a1,
                             const Arg2 &a2) {
        TranslateToExpression<Expression<Base>> translator(symtab);
        return mirv::make<NodeType>(a1,
                                    boost::fusion::transform(
                                      boost::fusion::pop_front(a2),
                                      translator));
      }

      template<typename Arg1, typename Arg2, typename Expr>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg1 a1,
                             Arg2 a2,
                             const Expr &expr) {
        TranslateToExpression<Expression<Base>> translator(symtab);
        return mirv::make<NodeType>(a1,
                                    a2,
                                    boost::fusion::transform(
                                      boost::fusion::pop_front(expr),
                                      translator));
      }
    };
  }
}

#endif
