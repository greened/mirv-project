#ifndef mirv_Core_Builder_FunctionGrammar_hpp
#define mirv_Core_Builder_FunctionGrammar_hpp

#include <mirv/Core/Builder/FunctionRules.hpp>
#include <mirv/Core/Builder/VariableGrammar.hpp>
#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Builder/CallExpressionGrammar.hpp>
#include <mirv/Core/Builder/StatementGrammar.hpp>
#include <mirv/Core/Builder/TypeLookupGrammar.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to get the current function.
    struct GetFunction : boost::proto::callable {
      typedef ptr<Function> result_type;

      template<typename T>
      result_type operator()(ptr<SymbolTable> symtab, T) {
        std::cerr << "GetFunction\n";
	result_type function = symtab->getFunction();
	return function;
      }
    };

    /// This is a callable transform to add a statement to a function.
    struct AddFunctionStatement : boost::proto::callable {
      typedef ptr<Block> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
			     ptr<Control> statement) {
        std::cerr << "AddFunctionStatement\n";
        auto function = symtab->getFunction();
        if (!function->getBlock()) {
          function->setBlock(symtab->getCurrentScope().getBlock());
        }
	function->getBlock()->push_back(statement);
	return function->getBlock();
      }
    };

    /// This is a callable transform to add a block statement to a
    /// function.
    struct AddFunctionBodyForVariable : boost::proto::callable {
      typedef ptr<Block> result_type;

      result_type operator()(ptr<SymbolTable> symtab,
                             ptr<Producer> Alloc) {
        std::cerr << "AddFunctionBodyForVariable\n";
	auto function = symtab->getFunction();
        if (!function->getBlock()) {
          function->setBlock(symtab->getCurrentScope().getBlock());
        }
	return function->getBlock();
      }
    };

    /// This is a callable transform to get the current function body.
    struct GetFunctionBody : boost::proto::callable {
      typedef ptr<Block> result_type;

      result_type operator()(ptr<SymbolTable> symtab) {
        std::cerr << "GetFunctionBody\n";
	auto function = symtab->getFunction();
        if (!function->getBlock()) {
          function->setBlock(symtab->getCurrentScope().getBlock());
        }
	return function->getBlock();
      }
    };

    /// Given a statement, add it to the function.
    struct FunctionStatementBuilder : boost::proto::when<
      ConstructStatementGrammar,
      AddFunctionStatement(boost::proto::_data,
                           ConstructStatementGrammar(boost::proto::_))
      > {};

    /// Given a variable declaration add it to the function, creating
    /// a block statement if it does not already exist to hold its
    /// allocation statement.
    struct FunctionVariableBuilder : boost::proto::when<
      VariableRule,
      AddFunctionBodyForVariable(boost::proto::_data,
                                 VariableBuilder)
      > {};

  /// This is the grammar to build a variable, call or general
  /// statement.
  struct VariableCallOrStatementBuilder : boost::proto::or_<
    FunctionVariableBuilder,
    FunctionStatementBuilder
    > {};

    /// This is the grammar for function bodies.  It can contain
    /// variable declarations and statements.  We add variables and
    /// statements as we find them.  Variables are handled by the
    /// variable rule so we need only worry about statements.
    struct FunctionBodyBuilder : boost::proto::when<
      boost::proto::or_<
        VariableCallOrStatementBuilder,
        boost::proto::comma<
          FunctionBodyBuilder,
          VariableCallOrStatementBuilder
          >
        >,
      GetFunctionBody(boost::proto::_data)
      > {};

    /// This is the grammar for function declarations.
    struct FunctionDeclBuilder : boost::proto::when<
      FunctionDeclRule,
      GetFunction(
        boost::proto::_data,
        SetFunction(
          boost::proto::_data,
          BinaryConstructSymbol<Function, ModuleScope>(
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
        PopScope(
          FunctionBodyBuilder(
            // Function body
            boost::proto::_right,
            boost::proto::_state,
            PushScope(
              SetFunction(
                boost::proto::_data,
                BinaryConstructSymbol<Function, ModuleScope>(
                  boost::proto::_data,
                  // Function name
                  boost::proto::_value(
                    boost::proto::_right(
                      boost::proto::_left(boost::proto::_left(
                                            boost::proto::_left)))),
                  // Function type
                  FunctionTypeAccessBuilder(
                    boost::proto::_right(boost::proto::_left)))))),
          boost::proto::_data))
      > {};

    namespace {
      /// A function "operator."  This is a protoized object that
      /// implements the subscript operator to kick off function
      /// symbol generation.
      const FuncTerminal func = {{}};
    }
  }
}

#endif
