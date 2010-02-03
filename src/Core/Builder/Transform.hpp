#ifndef mirv_Core_Builder_Transform_hpp
#define mirv_Core_Builder_Transform_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Builder/Make.hpp>

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
      typedef mirv::ptr<mirv::Symbol<mirv::Module> >::type ModulePointer;
      ModulePointer module;
      typedef mirv::ptr<mirv::Symbol<mirv::Function> >::type FunctionPointer;
      FunctionPointer function;

    public:
      SymbolTable(ModulePointer m, FunctionPointer f)
	: module(m), function(f) {}


      void setFunction(FunctionPointer f) {
	function = f;
      }
      void clearFunction(void) {
	function.reset();
      }

      template<typename SymbolType> struct Key {};

      /// Get the variable symbol at the current scope only.  Return a
      /// null pointer if the symbol does not exist.
      ptr<Symbol<Variable> >::type
      lookupAtCurrentScope(const std::string &name,
			   Key<Symbol<Variable> >) const {
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
			   Key<Symbol<Function> >) const {
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
			   Key<Symbol<Type<TypeBase> > >) const {
	Symbol<Module>::TypeIterator i = module->typeFind(name);
	if (i != module->typeEnd()) {
	  return *i;
	}
	return ptr<Symbol<Type<TypeBase> > >::type();
      }

      ptr<Symbol<Variable> >::type
      lookupAtAllScopes(const std::string &name,
			Key<Symbol<Variable> >) const {
	ptr<Symbol<Variable> >::type var =
	  lookupAtCurrentScope(name, Key<Symbol<Variable> >());
	if (function && !var) {
	  // Look up at module scope
	  SymbolTable ModuleScope(module, ptr<Symbol<Function> >::type());
	  var = ModuleScope.lookupAtCurrentScope(name, Key<Symbol<Variable> >());
        }
	if (!var) {
	  error("Could not find variable");
	}
	return var;
      } 
     
      ptr<Symbol<Function> >::type
      lookupAtAllScopes(const std::string &name,
			Key<Symbol<Function> >) const {
	ptr<Symbol<Function> >::type function =
	  lookupAtCurrentScope(name, Key<Symbol<Function> >());
	if (!function) {
	  error("Could not find function");
	}
	return function;
      }

      ptr<Symbol<Type<TypeBase> > >::type
      lookupAtAllScopes(const std::string &name,
			Key<Symbol<Type<TypeBase> > >) const {
	ptr<Symbol<Type<TypeBase> > >::type type =
	  lookupAtCurrentScope(name, Key<Symbol<Type<TypeBase> > >());
        if (!type) {
	  error("Could not find type");
	}
        return type;
      }

      void addAtCurrentScope(ptr<Symbol<Variable> >::type var) {
	ptr<Symbol<Variable> >::type result =
	  lookupAtCurrentScope(var->name(), Key<Symbol<Variable> >());
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
	  lookupAtCurrentScope(func->name(), Key<Symbol<Function> >());
        if (result) {
	  error("Function already exists");
	}
        module->functionPushBack(func);
      }

      void addAtCurrentScope(ptr<Symbol<Type<TypeBase> > >::type type) {
	ptr<Symbol<Type<TypeBase> > >::type result =
	  lookupAtCurrentScope(type->name(), Key<Symbol<Type<TypeBase> > >());
        if (result) {
	  error("Type already exists");
	}
        module->typePushBack(type);
      }
    };
    
    /// This is a callable transform to set the function scope in a
    /// symbol table.
    struct SetFunction : boost::proto::callable {
      typedef SymbolTable result_type;

      SymbolTable operator()(const SymbolTable &symtab,
			     ptr<Symbol<Function> >::type function) {
	result_type result(symtab);
	result.setFunction(function);
        return result;
      }
    };

    /// This is a callable transform to lookup a symbol.
    template<typename SymbolType,
	     typename Dummy = boost::proto::callable>
    struct LookupSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      result_type operator()(const SymbolTable &symtab,
			     const std::string &name) {
	result_type result = symtab.lookupAtAllScopes(name, SymbolTable::Key<SymbolType>());
	if (!result) {
	  error("Symbol does not exist");
	}
        return result;
      }
    };

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
