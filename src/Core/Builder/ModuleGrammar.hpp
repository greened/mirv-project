#ifndef mirv_Core_Builder_ModuleGrammar_hpp
#define mirv_Core_Builder_ModuleGrammar_hpp

#include <mirv/Core/Builder/ModuleRules.hpp>
#include <mirv/Core/Builder/FunctionGrammar.hpp>
#include <mirv/Core/Builder/TypeGrammar.hpp>
#include <mirv/Core/Builder/VariableGrammar.hpp>
#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/IR/Module.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is a callable transform to get the current module.
    struct GetModule : boost::proto::callable {
      typedef ptr<Symbol<Module> >::type result_type;

      template<typename T>
      result_type operator()(boost::shared_ptr<SymbolTable> symtab, T) {
	result_type module = symtab->getModule();
	return module;
      }
    };

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
    struct ModuleBuilder : boost::proto::when<
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
      > {};

    namespace {
      /// A module "operator."  This is a protoized object that
      /// implements the subscript operator to kick off module symbol
      /// generation.
      const ModuleTerminal module = {{}};
    }
  }
}

#endif
