#ifndef mirv_Core_Builder_SymbolGrammar_hpp
#define mirv_Core_Builder_SymbolGrammar_hpp

#include <mirv/Core/Builder/Symbol.hpp>

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

      result_type operator()(const SymbolTable &symtab,
			     size_t size) {
	std::string name = detail::GetBaseName<SymbolType>::value;
	name += boost::lexical_cast<std::string>(size);

	// Make sure we're not already in the symbol table at the current scope.
	result_type exists = symtab.lookupAtCurrentScope(name, SymbolTable::Key<SymbolType>());
	if (exists) {
	  error("Symbol exists");
	}
	result_type result = make<SymbolType>(name, size);
	symtab.addAtCurrentScope(result);
      }
    };

    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<typename SymbolType,
	     typename Dummy = boost::proto::callable>
    struct ConstructBinarySymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      result_type operator()(const SymbolTable &symtab,
			     const std::string &name,
			     typename ptr<Symbol<Type<TypeBase> > >::type type) {
	// Make sure we're not already in the symbol table at the current scope.
	result_type exists = symtab.lookupAtCurrentScope(name, SymbolTable::Key<SymbolType>());
	if (exists) {
	  error("Symbol exists");
	}
	result_type result = make<SymbolType>(name, type);
	symtab.addAtCurrentScope(result);
      }
    };

    /// This is a callable transform to construct a void type.
    struct ConstructVoidType : boost::proto::callable {
      typedef ptr<Symbol<Type<TypeBase> > >::type result_type;
      result_type operator()(void) {
	return result_type();
      }
    };

    /// This is a callable transform to construction a type list with
    /// a single member type.
    struct ConstructTypeList : boost::proto::callable {
      typedef std::list<ptr<Symbol<Type<TypeBase> > >::type> result_type;
      result_type operator()(ptr<Symbol<Type<TypeBase> > >::type type) {
	result_type typelist;
	typelist.push_back(type);
	return typelist;
      }
    };

    /// This is a callable transform to aggregate a list of types.
    struct AddToTypeList : boost::proto::callable {
      typedef std::list<ptr<Symbol<Type<TypeBase> > >::type> result_type;
      result_type operator()(result_type &typelist,
			     ptr<Symbol<Type<TypeBase> > >::type type) {
	// TODO: Check for duplicates.
	typelist.push_back(type);
	return typelist;
      }
    };

  // Define the symbol grammar.
  struct ConstructSymbolGrammar;

  struct ConstructSymbolGrammarCases {
    /// This is the default case.  It matches nothing, ensuring that
    /// illegal oncstructs flag an error.
    template<typename Tag>
    struct case_ : boost::proto::not_<boost::proto::_> {};
  };

      /// This is the grammar for module symbols.
    typedef boost::proto::when<
      ModuleRule,
      ConstructSymbolGrammar(boost::proto::_right,
			     boost::proto::_state,
			     ConstructUnary<SymbolTable, ptr<Symbol<Module> >::type>(ConstructUnary<Symbol<Module>, const std::string &>(boost::proto::_value(boost::proto::_right(boost::proto::_left)))))> ModuleBuilder;

    /// This is the grammar for function symbols.
    typedef boost::proto::when<
      FunctionRule,
      // TODO: Need to set the function body
      ConstructSymbolGrammar(boost::proto::_right,
			     boost::proto::_state,
			     SetFunction(boost::proto::_data,
					 ConstructBinarySymbol<
					 Symbol<
					 Function> 
					 >(boost::proto::_data,
					   // Function name
					   boost::proto::_right(boost::proto::_left(boost::proto::_left(boost::proto::_left))),
					   // Function type
					   LookupSymbol<Symbol<Type<TypeBase> > >(boost::proto::_data,
									     boost::proto::_right(boost::proto::_left)))))> FunctionBuilder;

    /// This is the grammar for variable symbols.
  typedef boost::proto::when<
    VariableRule,
    ConstructBinarySymbol<
      Symbol<
	Variable> >(boost::proto::_data,
		    // Variable name
		    boost::proto::_right(boost::proto::_left(boost::proto::_left)),
		    // Variable type
		    LookupSymbol<Symbol<Type<TypeBase> > >(boost::proto::_data,
							   boost::proto::_right))> VariableBuilder;

  template<>
  struct ConstructSymbolGrammarCases::case_<boost::proto::tag::subscript>
    : boost::proto::or_<ModuleBuilder,
			FunctionBuilder,
			VariableBuilder> {};

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
    struct ConstructSymbolGrammar
      : boost::proto::switch_<ConstructSymbolGrammarCases> {};
  }
}

#endif
