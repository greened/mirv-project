#ifndef mirv_Core_Builder_SymbolGrammar_hpp
#define mirv_Core_Builder_SymbolGrammar_hpp

#include <mirv/Core/Builder/Symbol.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/proto/proto.hpp>
#include <boost/mpl/print.hpp>
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
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
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
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
			     Arg1 a1,
			     Arg2 a2) {
	std::string name = SymbolType::getName(a1, a2);

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
    class TranslateToSymbol : boost::proto::callable {
    private:
      boost::shared_ptr<SymbolTable> symtab;

    public:
      TranslateToSymbol<SymbolType>(boost::shared_ptr<SymbolTable> s)
      : symtab(s) {}

     typedef typename ptr<SymbolType>::type result_type;

      template<typename Expr>
      result_type operator()(const Expr &e) const {
        //std::cout << "Translating:\n";
        //boost::proto::display_expr(e);
        return safe_cast<SymbolType>(translate(e, symtab));
      }
    };

    /// This is a callable transform to construct a function type.
    struct ConstructFunctionTypeSymbol : boost::proto::callable {
      typedef ptr<Symbol<Type<FunctionType> > >::type result_type;

      template<typename Arg1, typename Arg2>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             Arg1 a1,
                             Arg2 a2) {
        TranslateToSymbol<Symbol<Type<TypeBase> > > translator(symtab);
        return BinaryConstructSymbol<Symbol<Type<FunctionType> > >()(
          symtab, a1, boost::fusion::transform(
            boost::fusion::pop_front(a2), translator));
      }
    };

    namespace detail {
      template<typename ResultType, bool Matches>
      class TranslateListImpl {
      public:
        template<typename List>
        ResultType operator()(boost::shared_ptr<SymbolTable> symtab,
                              const List typeList) {
          TranslateToSymbol<Symbol<Type<TypeBase> > > translator(symtab);
          return boost::fusion::transform(
            boost::proto::flatten(typeList), translator);
        }
      };

      template<typename ResultType>
      class TranslateListImpl<ResultType, false> {
      public:
        template<typename List>
        ResultType operator()(boost::shared_ptr<SymbolTable> symtab,
                              const List typeList) {
          return TranslateToSymbol<Symbol<Type<TypeBase> > >(symtab)(typeList);
        }
      };
      
      template<typename List>
      class TranslateList {
      public:
        typedef typename boost::mpl::if_<
        boost::proto::matches<List, StrictTypeList>,
        typename boost::fusion::result_of::transform<
          const typename boost::proto::result_of::flatten<const List>::type,
          TranslateToSymbol<Symbol<Type<TypeBase> > > >::type,
        ptr<Symbol<Type<TypeBase> > >::type
        >::type result_type;

        result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                               const List theList) {
          return TranslateListImpl<result_type,
            boost::proto::matches<List, StrictTypeList>::value>()(symtab, 
                                                                  theList);
        }
      };

      template<typename List>
      typename TranslateList<List>::result_type
      translateList(boost::shared_ptr<SymbolTable> symtab, List typeList) {
        return TranslateList<List>()(symtab, typeList);
      }
    }

    /// This is a callable transform to construct a struct type.
    struct ConstructStructTypeSymbol : boost::proto::callable {
      typedef ptr<Symbol<Type<StructType> > >::type result_type;

      template<typename List>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             const std::string &name,
                             List memberList) {
        //std::cout << "Building struct:\n";
        //boost::proto::display_expr(memberList);

        return BinaryConstructSymbol<Symbol<Type<StructType> > >()(
          symtab, name, detail::translateList(symtab, memberList));
      }
    };

    /// This is a callable transform to get the current module.
    struct GetModule : boost::proto::callable {
      typedef ptr<Symbol<Module> >::type result_type;

      template<typename T>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab, T) {
	result_type module = symtab->getModule();
	return module;
      }
    };

    /// This is a callable transform to get the current function.
    struct GetFunction : boost::proto::callable {
      typedef ptr<Symbol<Function> >::type result_type;

      template<typename T>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab, T) {
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

    // Define the symbol grammar.
    struct ConstructSymbolGrammar;

    /// This is the grammar for void types.
    typedef boost::proto::when<
      VoidTerminal,
      ConstructVoidType()
      > VoidBuilder;

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

    // struct FunctionTypeBuilder : boost::proto::or_<
    //   boost::proto::when<
    //     FunctionTypeWithArgsRule,
    //     LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
    //       boost::proto::_data,
    //       ConstructFunctionTypeSymbol(
    //         boost::proto::_data,
    //         FunctionReturnTypeBuilder(boost::proto::_left),
    //         boost::proto::_expr))
    //     >,
    //   boost::proto::when<
    //     FunctionTypeWithoutArgsRule,
    //     LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
    //       boost::proto::_data,
    //       UnaryConstructSymbol<Symbol<Type<FunctionType> > >(
    //         boost::proto::_data,
    //         FunctionReturnTypeBuilder(boost::proto::_left)))
    //     >
    //   > {};

    struct FunctionTypeBuilder : boost::proto::when<
      FunctionTypeWithArgsRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        ConstructFunctionTypeSymbol(
          boost::proto::_data,
          FunctionReturnTypeBuilder(boost::proto::_left),
          boost::proto::_expr))
      > {};

  // This is the grammar for struct types.
    struct StructTypeBuilder : boost::proto::when<
      StructTypeRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        ConstructStructTypeSymbol(
          boost::proto::_data,
          // Struct name
          boost::proto::_value(boost::proto::_right(boost::proto::_left)),
          // Member type list
          boost::proto::_right))
      > {};
  
    /// This is the grammar to construct types.
    struct TypeBuilder : boost::proto::or_<
      IntBuilder,
      FloatBuilder,
      FunctionTypeBuilder,
      StructTypeBuilder
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

    // This is the grammar to match a variable declaration, a type
    // definition or a function definition.
    struct VariableTypeOrFunctionBuilder : boost::proto::or_<
      VariableBuilder,
      TypeBuilder,
      FunctionBuilder
      > {};

    /// This is the grammar for module bodies.  It can contain
    /// variable declarations, type declarations and functions .  We
    /// add symbols as we find them.
    struct ModuleBodyBuilder : boost::proto::or_<
       VariableTypeOrFunctionBuilder,
       boost::proto::comma<
	 ModuleBodyBuilder,
	 VariableTypeOrFunctionBuilder
	 >
       > {};

    /// This is the grammar for module symbols.
    typedef boost::proto::when<
      ModuleRule,
      GetModule(
        boost::proto::_data,
        ModuleBodyBuilder(
          boost::proto::_right,
          boost::proto::_state,
          SetModule(
            boost::proto::_data,
            ConstructUnary<Symbol<Module>, const std::string &>(
              boost::proto::_value(boost::proto::_right(
                                     boost::proto::_left))))))
      > ModuleBuilder;

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
