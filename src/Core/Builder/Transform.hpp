#ifndef mirv_Core_Builder_Transform_hpp
#define mirv_Core_Builder_Transform_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>

#include <boost/proto/proto.hpp>

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

    public:
      SymbolTable(ModulePointer m, FunctionPointer f)
	: module(m), function(f) {}

      static ptr<SymbolTable>::type make(ModulePointer m) {
        ptr<SymbolTable>::type result(new SymbolTable(m, FunctionPointer()));
        return result;
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
      ptr<Symbol<Type<TypeBase> > >::type
      lookupAtCurrentScope(const std::string &name,
			   Symbol<Type<TypeBase> > *) const {
	Symbol<Module>::TypeIterator i = module->typeFind(name);
	if (i != module->typeEnd()) {
	  return *i;
	}
	return ptr<Symbol<Type<TypeBase> > >::type();
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

      ptr<Symbol<Type<TypeBase> > >::type
      lookupAtAllScopes(const std::string &name,
			Symbol<Type<TypeBase> > *) const {
	ptr<Symbol<Type<TypeBase> > >::type type =
	  lookupAtCurrentScope(name,
                               reinterpret_cast<Symbol<Type<TypeBase> > *>(0));
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

      void addAtCurrentScope(ptr<Symbol<Type<TypeBase> > >::type type) {
	ptr<Symbol<Type<TypeBase> > >::type result =
	  lookupAtCurrentScope(type->name(),
                               reinterpret_cast<Symbol<Type<TypeBase> > *>(0));
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

      /// Transform a one-operand node into a single-child IR node.
     template<typename NodeType,
	      typename Child = typename NodeType::ChildPtr,
	      typename Dummy = boost::proto::callable>
      struct ConstructUnary : boost::proto::callable {
	typedef typename ptr<NodeType>::type result_type;

	result_type operator()(Child child) {
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

	result_type operator()(Child1 left, Child2 right) {
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

       result_type operator()(Statement<Block>::ChildPtr left,
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

	result_type operator()(Child1 child1, Child2 child2, Child3 child3) {
	  return make<NodeType>(child1, child2, child3);
         }
      };

#if 0
      // Transform for an n-ary expression
      template<typename NodeType, typename Dummy = boost::proto::callable>
      struct ConstructNary
            : boost::proto::callable {
         template<typename Sig>
         struct result;

         template<typename This, typename Expr, typename State, typename Visitor>
         struct result<This(Expr, State, Visitor)> {
            typedef Ptr<NodeType>::type type;
         };

         template<typename Expr, typename State, typename Visitor>
         struct add_arg {

         private:
            typedef  typename result<ConstructNary<NodeType, Dummy>, Expr, State, Visitor>::type call_type;

            typedef typename Grammar::apply<Expr, State, Visitor>::type
            transformed_type;

         public:
            add_arg(call_type &c,
                    transformed_type &e)
                  : call(c), transformed_expr(e) {};
            template<typename T>
            void operator()(T) {
               call->addOperand(
                  boost::proto::arg<T::value>(transformed_expr));
            }

         private:
            call_type &call;
            transformed_type &transformed_expr;
         };

         template<typename Expr, typename State, typename Visitor>
         static typename result<ConstructNary<NodeType, Dummy>, Expr, State, Visitor>::type
         operator()(Expr const &expr, State const &state, Visitor &visitor) {
            typedef typename result<ConstructNary<NodeType, Dummy>, Expr, State, Visitor>::type ptr_type;

            ptr_type result(new NodeType(
                               // Function
                               boost::proto::_arg0));

            // Call FCall::addOperand for each operand
            typedef boost::mpl::range_c<
            int,
               1,
               boost::proto::arity_of<Expr>::value> args;
            boost::mpl::for_each<args>(
               add_arg<Expr, State, Visitor>(result, transformed_expr));
            return(result);
         }
      };
#endif
   }
}

#endif