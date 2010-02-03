#ifndef mirv_Core_Builder_SymbolGrammar_hpp
#define mirv_Core_Builder_SymbolGrammar_hpp

#include <mirv/Core/Builder/Symbol.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
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

    /// This aggregates all of the symbol rules.  It serves as the
    /// grammar for all statements.
    struct ConstructSymbolGrammar
      : boost::proto::switch_<ConstructSymbolGrammarCases> {};
  }
}

#endif
