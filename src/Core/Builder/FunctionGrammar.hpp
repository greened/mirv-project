#ifndef mirv_Core_Builder_FunctionGrammar_hpp
#define mirv_Core_Builder_FunctionGrammar_hpp

#include <mirv/Core/Builder/FunctionRules.hpp>
#include <mirv/Core/Builder/VariableGrammar.hpp>
#include <mirv/Core/Builder/TypeLookupGrammar.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/IR/Function.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to get the current function.
    struct GetFunction : boost::proto::callable {
      typedef ptr<Symbol<Function> >::type result_type;

      template<typename T>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab, T) {
	result_type function = symtab->getFunction();
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

    /// This is a callable transform to add a block statement to a
    /// function.
    struct AddFunctionBodyForVariable : boost::proto::callable {
      typedef ptr<Symbol<Function> >::type result_type;

      result_type operator()(boost::shared_ptr<SymbolTable> symtab,
                             boost::shared_ptr<Symbol<Variable>>) {
	result_type function = symtab->getFunction();
        if (function->statementEmpty()) {
          function->statementPushBack(make<Statement<Block>>());
        }
	return function;
      }
    };

    struct FunctionStatementBuilder : boost::proto::when<
      ConstructStatementGrammar,
      AddFunctionStatement(boost::proto::_data,
                           ConstructStatementGrammar(boost::proto::_))
      > {};

    struct VariableOrStatementBuilder : boost::proto::or_<
      boost::proto::when<
        VariableRule,
        AddFunctionBodyForVariable(boost::proto::_data,
                                   VariableBuilder)
        >,
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

    /// This is the grammar for function declarations.
    struct FunctionDeclBuilder : boost::proto::when<
      FunctionDeclRule,
      GetFunction(
        boost::proto::_data,
        SetFunction(
          boost::proto::_data,
          BinaryConstructSymbol<Symbol<Function> >(
            boost::proto::_data,
            // Function name
            boost::proto::_value(
              boost::proto::_right(
                boost::proto::_left(boost::proto::_left))),
            // Function type
            FunctionTypeAccessBuilder(
              boost::proto::_right))))
      > {};

    /// This is the grammar for function symbols.
    struct FunctionBuilder : boost::proto::when<
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
      > {};

    namespace {
      /// A function "operator."  This is a protoized object that
      /// implements the subscript operator to kick off function
      /// symbol generation.
      const FunctionTerminal function = {{}};
    }    
  }
}

#endif
