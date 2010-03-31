#ifndef mirv_Core_Builder_SymbolGrammar_hpp
#define mirv_Core_Builder_SymbolGrammar_hpp

#include <mirv/Core/Builder/Symbol.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/proto/proto.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/type_traits.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to construct a symbol.  If the
    /// symbol exists at the current scope, it is an error.
    template<typename SymbolType,
	     typename Dummy = boost::proto::callable>
    struct UnaryConstructSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      template<typename Arg>
      result_type operator()(ptr<SymbolTable>::type symtab,
			     Arg a) {
	std::string name = SymbolType::getName(a);

	// Make sure we're not already in the symbol table at the current scope.
	ptr<Symbol<Base> >::type exists =
          symtab->lookupAtCurrentScope(name, reinterpret_cast<SymbolType *>(0));
	if (exists) {
          if (boost::is_base_and_derived<Symbol<Type<TypeBase> >,
              SymbolType>::value) {
            // It's ok to have a type already declared.
            return safe_cast<SymbolType>(exists);
          }
	  error("Symbol exists");
	}
	result_type result = make<SymbolType>(a);
	symtab->addAtCurrentScope(result);
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
      result_type operator()(ptr<SymbolTable>::type symtab,
			     Arg1 a1,
			     Arg2 a2) {
	std::string name = SymbolType::getName(a1, a2);

	// Make sure we're not already in the symbol table at the current scope.
	result_type exists =
          symtab->lookupAtCurrentScope(name, reinterpret_cast<SymbolType *>(0));
	if (exists) {
          if (boost::is_base_and_derived<Symbol<Type<TypeBase> >,
              SymbolType>::value) {
            // It's ok to have a type already declared.
            return safe_cast<SymbolType>(exists);
          }
	  error("Symbol exists");
	}
	result_type result = make<SymbolType>(a1, a2);
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

    /// This is a callable transform to translate a proto expression
    /// to a symbol.
    template<typename SymbolType>
    struct TranslateToSymbol : boost::proto::callable {
      typedef typename ptr<SymbolType>::type result_type;

      template<typename Expr>
      result_type operator()(const Expr &e) const {
        return translate(e);
      }
    };

    /// This is a callable transform to construct a function type.
    struct ConstructFunctionTypeSymbol : boost::proto::callable {
      typedef ptr<Symbol<Type<FunctionType> > >::type result_type;

      template<typename Arg1, typename Arg2>
      result_type operator()(ptr<SymbolTable>::type symtab, Arg1 a1, Arg2 a2) {
        return BinaryConstructSymbol<Symbol<Type<FunctionType> > >(
          symtab, "", a1, boost::fusion::transform(
            boost::fusion::pop_front(a2),
            TranslateToSymbol<Symbol<Type<FunctionType> > >()));
      }
    };

    /// This is a callable transform to get the current module.
    struct GetModule : boost::proto::callable {
      typedef ptr<Symbol<Module> >::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     boost::shared_ptr<Node<Base> >) {
	result_type module = symtab->getModule();
	return module;
      }
    };

    /// This is a callable transform to get the current function.
    struct GetFunction : boost::proto::callable {
      typedef ptr<Symbol<Function> >::type result_type;

      template<typename T>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     T) {
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
      GetModule(
        boost::proto::_data,
        ConstructSymbolGrammar(
          boost::proto::_right,
          boost::proto::_state,
          SetModule(
            boost::proto::_data,
            ConstructUnary<Symbol<Module>, const std::string &>(
              boost::proto::_value(boost::proto::_right(
                                     boost::proto::_left))))))
      > ModuleBuilder;

    /// This is the grammar for void types.
    typedef boost::proto::when<
      VoidTerminal,
      ConstructVoidType()
      > VoidBuilder;

    /// This is the grammar for type lists.
    // TODO: Use fold_tree<>.
    typedef boost::proto::when<
      TypeListListPart,
      AddToTypeList(ConstructSymbolGrammar(boost::proto::_left),
		    ConstructSymbolGrammar(boost::proto::_right))
      > TypeListListPartBuilder;

    typedef boost::proto::or_<
      boost::proto::when<
	TypeRule,
	ConstructTypeList(ConstructSymbolGrammar(boost::proto::_))
	>,
      TypeListListPartBuilder
      > OldTypeListBuilder;

    struct TypeListBuilder : boost::proto::when<
      TypeList,
      boost::proto::functional::flatten(
        ConstructSymbolGrammar(boost::proto::_))
      > {};

    struct FunctionTypeBuilder;

    /// This is the grammar for integral types.
    typedef boost::proto::when<
      IntRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        UnaryConstructSymbol<Symbol<Type<Integral> > >(
          boost::proto::_data,
          boost::proto::_value(boost::proto::_right)))
      > IntBuilder;

    /// This is the grammar for floating point types.
    typedef boost::proto::when<
      FloatRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        UnaryConstructSymbol<Symbol<Type<Floating> > >(
          boost::proto::_data,
          boost::proto::_value(boost::proto::_right)))
      > FloatBuilder;

    struct TypeBuilder;

    /// This is the grammar for function types.
    struct FunctionReturnTypeBuilder : boost::proto::or_<
      VoidBuilder,
      TypeBuilder
      > {};

    struct FunctionTypeBuilder : boost::proto::or_<
      boost::proto::when<
	FunctionTypeWithArgsRule,
	LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
          boost::proto::_data,
          ConstructFunctionTypeSymbol(
            boost::proto::_data,
            FunctionReturnTypeBuilder(boost::proto::_left),
            boost::proto::_expr))
        >,
      boost::proto::when<
        FunctionTypeWithoutArgsRule,
        LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
          boost::proto::_data,
          UnaryConstructSymbol<Symbol<Type<FunctionType> > >(
            boost::proto::_data,
            FunctionReturnTypeBuilder(boost::proto::_left)))
        >
      > {};
  
    /// This is the grammar to construct types.
    struct TypeBuilder : boost::proto::or_<
      IntBuilder,
      FloatBuilder,
      FunctionTypeBuilder
      > {};

    /// This is the grammar to look up a type.  In the case of
    /// constructed types, see if it already exists in the symbol table
    /// and add it if not.
    typedef boost::proto::or_<
      boost::proto::when<
        StringTerminal,
        LookupSymbol<Symbol<Type<TypeBase> > >(
          boost::proto::_data,
          boost::proto::_value)
        >,
      TypeBuilder
      > TypeAccessBuilder;

    typedef boost::proto::or_<
      boost::proto::when<
        StringTerminal,
        LookupSymbol<Symbol<Type<TypeBase> > >(
          boost::proto::_data,
          boost::proto::_value)
        >,
      FunctionTypeBuilder
      > FunctionTypeAccessBuilder;

    /// This is the grammar for variable symbols.
    typedef boost::proto::when<
      VariableRule,
      BinaryConstructSymbol<
        Symbol<Variable> >(
          boost::proto::_data,
          // Variable name
          boost::proto::_value(boost::proto::_right(
                                 boost::proto::_left(boost::proto::_left))),
          // Variable type
          TypeAccessBuilder(boost::proto::_right))
      > VariableBuilder;

    typedef boost::proto::when<
      ConstructStatementGrammar,
      AddFunctionStatement(boost::proto::_data,
                           ConstructStatementGrammar(boost::proto::_))
      > FunctionStatementBuilder;

    struct VariableOrStatementBuilder : boost::proto::or_<
      VariableBuilder,
      FunctionStatementBuilder
      > {};

    /// This is the grammar for function bodies.  It can contain
    /// variable declarations and statements.  We add variables and
    /// statements as we find them.  Variables are handled by the
    /// variable rule so we need only worry about statements.
    struct FunctionBodyBuilder : boost::proto::or_<
       VariableOrStatementBuilder,
       boost::proto::comma<
	 FunctionBodyBuilder,
	 VariableOrStatementBuilder
	 >
       > {};

    /// This is the grammar for function symbols.
    typedef boost::proto::when<
      FunctionRule,
      GetFunction(
        boost::proto::_data,
        FunctionBodyBuilder(
          // Function body
          boost::proto::_right,
          boost::proto::_state,
          SetFunction(
            boost::proto::_data,
            BinaryConstructSymbol<Symbol<Function> >(
              boost::proto::_data,
              // Function name
              boost::proto::_value(
                boost::proto::_right(
                  boost::proto::_left(boost::proto::_left(
                                        boost::proto::_left)))),
              // Function type
              FunctionTypeAccessBuilder(
                boost::proto::_right(boost::proto::_left))))))
      > FunctionBuilder;

    /// This aggregates all of the symbol rules.  It serves as the
    /// grammar for all symbols.
    struct ConstructSymbolGrammar : boost::proto::or_<
      ModuleBuilder,
      FunctionBuilder,
      VariableBuilder,
      TypeBuilder
      > {};
  }
}

#endif
