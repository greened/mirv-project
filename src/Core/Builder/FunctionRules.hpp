#ifndef mirv_Core_Builder_FunctionRules_hpp
#define mirv_Core_Builder_FunctionRules_hpp

#include <mirv/Core/Builder/VariableRules.hpp>
#include <mirv/Core/Builder/StatementGrammar.hpp>
#include <mirv/Core/Builder/TypeRules.hpp>
#include <mirv/Core/Builder/TypeLookupRules.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     //
     // Functions:
     //
     // function["name"].type[int_(32)(float_(64), int_(32))] [
     //   Body
     // ];

     /// Define a rule for a list of variables.
     struct VariableList :
       boost::proto::or_<
       VariableRule,
       boost::proto::comma<
	 VariableList,
	 VariableRule
	 >
       > {};

     /// Define a rule to match a variable declaration or a statement.
     struct VariableOrStatement : boost::proto::or_<
       VariableRule,
       ConstructStatementGrammar
       > {};

     /// Define a rule to match a list of variables and statements.
     struct VariableStatementList : boost::proto::or_<
       VariableOrStatement,
       boost::proto::comma<
	 VariableStatementList,
	 VariableOrStatement
	 >
       > {};

     /// This is the rule to match function declarations.  It
     /// matches function["name"].type["name"|type].
     struct FunctionDeclRule : boost::proto::subscript<
       boost::proto::member<
         boost::proto::subscript<
           FunctionTerminal,
           StringTerminal
           >,
         TypeTerminal
         >,
           FunctionTypeAccessRule
           > {};

     /// This is the rule to match function symbols.  It matches
     /// function["name"].type["name"|type][body].
     struct FunctionRule : boost::proto::subscript<
       boost::proto::subscript<
	 boost::proto::member<
	   boost::proto::subscript<
	     FuncTerminal,
	     StringTerminal
	     >,
	   TypeTerminal
	   >,
	 FunctionTypeAccessRule
	 >,
       VariableStatementList
    > {};
   }
}

#endif
