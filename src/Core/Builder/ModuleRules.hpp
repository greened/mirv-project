#ifndef mirv_Core_Builder_ModuleRules_hpp
#define mirv_Core_Builder_ModuleRules_hpp

#include <mirv/Core/Builder/SymbolTerminals.hpp>
#include <mirv/Core/Builder/FunctionRulesFwd.hpp>
#include <mirv/Core/Builder/TypeRulesFwd.hpp>
#include <mirv/Core/Builder/GlobalVariableRulesFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
  // Builder syntax
  //
  // Modules:
  //
  // module["name"] [
  //   Body
  // ];

  /// Define a rule to match a function, variable or type.
     struct FunctionVariableType : boost::proto::or_<
    FunctionRule,
    FunctionDeclRule,
    GlobalVariableRule,
    TypeRule
       > {};

     /// Define a rule for a list of functions, variables and types.
     struct FunctionVariableTypeList :
       boost::proto::or_<
       FunctionVariableType,
       boost::proto::comma<
	 FunctionVariableTypeList,
	 FunctionVariableType
	 > 
       > {};

     /// This is the rule to match module symbols.  It matches
     /// module["name"][body].
     struct ModuleRule : boost::proto::subscript<
       boost::proto::subscript<
	 ModuleTerminal,
	 StringTerminal
	 >,
       FunctionVariableTypeList
       > {};
   }
}

#endif
