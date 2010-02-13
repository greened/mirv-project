#ifndef mirv_Core_Builder_SymbolGrammar_hpp
#define mirv_Core_Builder_SymbolGrammar_hpp

#include <mirv/Core/Builder/Symbol.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/proto/proto.hpp>
#include <boost/lexical_cast.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      /// This computes the base name of a simple type at compile time.
      template<typename SymbolType>
      struct GetBaseName;
      template<>
      struct GetBaseName<Symbol<Type<Integral> > > {
	static const char *value;
      };
      template<>
      struct GetBaseName<Symbol<Type<Floating> > > {
	static const char *value;
      };
    }

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<typename SymbolType,
	     typename Dummy = boost::proto::callable>
    struct ConstructUnarySymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      result_type operator()(ptr<SymbolTable>::type symtab,
			     size_t size) {
	std::string name = detail::GetBaseName<SymbolType>::value;
	name += boost::lexical_cast<std::string>(size);

	// Make sure we're not already in the symbol table at the current scope.
	result_type exists = symtab->lookupAtCurrentScope(name, SymbolTable::Key<SymbolType>());
	if (exists) {
	  error("Symbol exists");
	}
	result_type result = make<SymbolType>(name, size);
	symtab->addAtCurrentScope(result);
	return result;
      }
    };

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<typename SymbolType,
	     typename Dummy = boost::proto::callable>
    struct ConstructBinarySymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      result_type operator()(ptr<SymbolTable>::type symtab,
			     const std::string &name,
			     boost::shared_ptr<Symbol<Type<TypeBase> > > type) {
	// Make sure we're not already in the symbol table at the current scope.
	result_type exists = symtab->lookupAtCurrentScope(name, SymbolTable::Key<SymbolType>());
	if (exists) {
	  error("Symbol exists");
	}
	result_type result = make<SymbolType>(name, type);
	symtab->addAtCurrentScope(result);
	return result;
      }
    };

    /// This is a callable transform to construct a void type.
    struct ConstructVoidType : boost::proto::callable {
      typedef ptr<Symbol<Type<TypeBase> > >::type result_type;
      result_type operator()(void) {
	return result_type();
      }
    };

    /// This is a callable transform to get the current function.
    struct GetFunction : boost::proto::callable {
      typedef ptr<Symbol<Function> >::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     boost::shared_ptr<Node<Base> >) {
	result_type function = symtab->getFunction();
	if (function->statementEmpty()) {
	  function->statementPushBack(make<Statement<Block> >());
	}
	checkInvariant(function->getStatement(), "No function statement");
	return function;
      }
    };

    /// This is a callable transform to add a statement to a function.
    struct AddFunctionStatement : boost::proto::callable {
      typedef ptr<Symbol<Function> >::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     boost::shared_ptr<Statement<Base> > body) {
	result_type function = symtab->getFunction();
	function->statementPushBack(body);
	return function;
      }
    };

    /// This is a callable transform to construct a type list with a
    /// single member type.
    struct ConstructTypeList : boost::proto::callable {
      typedef std::list<ptr<Symbol<Type<TypeBase> > >::type> result_type;
      result_type operator()(boost::shared_ptr<Symbol<Type<TypeBase> > > type) {
	result_type typelist;
	typelist.push_back(type);
	return typelist;
      }
    };

    /// This is a callable transform to aggregate a list of types.
    struct AddToTypeList : boost::proto::callable {
      typedef std::list<ptr<Symbol<Type<TypeBase> > >::type> result_type;
      result_type operator()(result_type &typelist,
			     boost::shared_ptr<Symbol<Type<TypeBase> > > type) {
	// TODO: Check for duplicates.
	typelist.push_back(type);
	return typelist;
      }
    };

  // Define the symbol grammar.
  struct ConstructSymbolGrammar;

    /// This is the grammar for module symbols.
    typedef boost::proto::when<
      ModuleRule,
      ConstructSymbolGrammar(boost::proto::_right,
			     boost::proto::_state,
			     ConstructUnary<SymbolTable, ptr<Symbol<Module> >::type>(ConstructUnary<Symbol<Module>, const std::string &>(boost::proto::_value(boost::proto::_right(boost::proto::_left)))))> ModuleBuilder;

    /// This is the grammar for variable symbols.
  typedef boost::proto::when<
    VariableRule,
    ConstructBinarySymbol<
      Symbol<
	Variable> >(boost::proto::_data,
		    // Variable name
		    boost::proto::_value(boost::proto::_right(boost::proto::_left(boost::proto::_left))),
		    // Variable type
		    LookupSymbol<Symbol<Type<TypeBase> > >(boost::proto::_data,
							   boost::proto::_value(boost::proto::_right)))> VariableBuilder;

    /// This is the grammar for function bodies.  It can contain
    /// variable declarations and statements.  We add variables and
    /// statements as we find them.  Variables are handled by the
    /// variable rule so we need only worry about statements.
    struct FunctionBodyBuilder : boost::proto::or_<
      VariableBuilder,
      boost::proto::when<ConstructStatementGrammar,
			 AddFunctionStatement(boost::proto::_data,
					      ConstructStatementGrammar(boost::proto::_))>
      > {};

    /// This is the grammar for function symbols.
    typedef boost::proto::when<
      FunctionRule,
      GetFunction(boost::proto::_data,
		  FunctionBodyBuilder(boost::proto::_right,
				      boost::proto::_state,
				      SetFunction(boost::proto::_data,
						  ConstructBinarySymbol<
						  Symbol<
						  Function> 
						  >(boost::proto::_data,
						    // Function name
						    boost::proto::_value(boost::proto::_right(boost::proto::_left(boost::proto::_left(boost::proto::_left)))),
						    // Function type
						    LookupSymbol<Symbol<Type<TypeBase> > >(boost::proto::_data,
											   boost::proto::_value(boost::proto::_right(boost::proto::_left)))))))> FunctionBuilder;

    /// This is the grammar for void types.
    typedef boost::proto::when<
      VoidTerminal,
      ConstructVoidType()> VoidBuilder;

    /// This is the grammar for type lists.
    typedef boost::proto::when<
      TypeListListPart,
      AddToTypeList(ConstructSymbolGrammar(boost::proto::_left),
		    ConstructSymbolGrammar(boost::proto::_right))> TypeListListPartBuilder;

    typedef boost::proto::or_<
      boost::proto::when<
	TypeRule,
	ConstructTypeList(ConstructSymbolGrammar(boost::proto::_))
	>,
      boost::proto::when<
	TypeListListPart,
	AddToTypeList(ConstructSymbolGrammar(boost::proto::_right),
		      ConstructSymbolGrammar(boost::proto::_left))
	> > TypeListBuilder;

    /// This is the grammar for function types.
    typedef boost::proto::when<
      FunctionTypeRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(boost::proto::_data,
						   ConstructBinarySymbol<Symbol<Type<FunctionType> > >(ConstructSymbolGrammar(boost::proto::_left),
												       ConstructSymbolGrammar(boost::proto::_right)))> FunctionTypeBuilder;

  /// This is the grammar for integral types.
  typedef boost::proto::when<
    IntRule,
    ConstructUnarySymbol<Symbol<Type<Integral> > >(ConstructSymbolGrammar(boost::proto::_right))> IntBuilder;

  /// This is the grammar for floating point types.
  typedef boost::proto::when<
    FloatRule,
    ConstructUnarySymbol<Symbol<Type<Floating> > >(ConstructSymbolGrammar(boost::proto::_right))> FloatBuilder;

  /// This is the grammar to look up a type.  In the case of
  /// constructed types, see if it already exists in the symbol table
  /// and add it if not.
  typedef boost::proto::or_<
    boost::proto::when<
      StringTerminal,
      LookupSymbol<Symbol<Type<TypeBase> > >(boost::proto::_value)
      >,
    boost::proto::when<
      TypeRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(ConstructGrammar(boost::proto::_))> > TypeAccessBuilder;

    /// This aggregates all of the symbol rules.  It serves as the
    /// grammar for all statements.
    struct ConstructSymbolGrammar : boost::proto::or_<
      ModuleBuilder,
      FunctionBuilder,
      VariableBuilder
      > {};
  }
}

#endif
