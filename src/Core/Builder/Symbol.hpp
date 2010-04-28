#ifndef mirv_Core_Builder_Symbol_hpp
#define mirv_Core_Builder_Symbol_hpp

#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/DomainFwd.hpp>
#include <mirv/Core/Builder/GrammarFwd.hpp>
#include <mirv/Core/Builder/Keywords.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/IR/Variable.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// A terminal for string constants.
     typedef Wrapper<boost::proto::terminal<boost::proto::convertible_to<std::string> >::type> StringTerminal;
     /// A terminal for integer constants.
     typedef Wrapper<boost::proto::terminal<int>::type> IntegerTerminal;

     // Types:
     //
     // struct_["name"] [
     //   int_(32),
     //   float_(64)
     // ];
     //

     typedef Wrapper<boost::proto::terminal<keyword::int_>::type> IntTerminal;

     /// This is the rule to match int type symbols.  It matches
     /// int_(int)
     typedef boost::proto::function<
       IntTerminal,
       IntegerTerminal
       > IntRule;

     typedef Wrapper<boost::proto::terminal<keyword::float_>::type> FloatTerminal;

     /// This is the rule to match float type symbols.  It matches
     /// float_(float)
     typedef boost::proto::function<
       FloatTerminal,
       IntegerTerminal
       > FloatRule;

     /// This is the rule to match void types.
     typedef Wrapper<boost::proto::terminal<keyword::void_>::type> VoidTerminal;

     /// This is the rule to match simple type symbols.  It matches
     /// int_(int)|float_(float)|void_
     typedef boost::proto::or_<
       IntRule,
       FloatRule
       > SimpleTypeRule;

     /// Define a rule for a list of types.
     struct TypeRule;

     struct TypeList : boost::proto::or_<
       TypeRule,
       boost::proto::comma<
         TypeList,
         TypeRule
         >
       > {};

     typedef Wrapper<boost::proto::terminal<keyword::struct_>::type> StructTerminal;

     /// This is the rule to match struct type symbols.  It matches
     /// struct_["name"][body]
     typedef boost::proto::subscript<
       boost::proto::subscript<
	 StructTerminal,
	 StringTerminal
	 >,
       TypeList
       > StructTypeRule;

     struct TypeRule;

     /// This is the rule to match function type symbols.  It matches
     /// Type|void_(TypeList)
     typedef boost::proto::function<
       boost::proto::or_<
	 VoidTerminal,
	 TypeRule
	 >,
       boost::proto::vararg<TypeRule>
       > FunctionTypeWithArgsRule;

     // typedef boost::proto::function<
     //   boost::proto::or_<
     //     VoidTerminal,
     //     TypeRule
     //     >
     //   > FunctionTypeWithoutArgsRule;

     // typedef boost::proto::or_<
     //   FunctionTypeWithArgsRule,
     //   FunctionTypeWithoutArgsRule
     //   > FunctionTypeRule;

     typedef FunctionTypeWithArgsRule FunctionTypeRule;

     /// This is the rule to  match type symbols.  It matches
     /// IntType|FloatType|StructType|FunctionType
     struct TypeRule : boost::proto::or_<
       SimpleTypeRule,
       StructTypeRule,
       FunctionTypeRule
       > {};

     /// Define a rule to access a type from a module.
     struct TypeAccessRule : boost::proto::or_<
       StringTerminal,
       TypeRule
       > {};

     /// Define a rule to access a function type from a module.
     struct FunctionTypeAccessRule : boost::proto::or_<
       StringTerminal,
       FunctionTypeRule
       > {};

     // Variables:
     //
     // var["name"].type[int_(32)];
     //

     typedef Wrapper<boost::proto::terminal<keyword::var>::type> VarTerminal;
     typedef Wrapper<boost::proto::terminal<keyword::type>::type> TypeTerminal;

     /// This is the rule to match variable symbols.  It matches
     /// var["name"].type["name"|type].
     struct VariableNameSpecifier : boost::proto::subscript<
       VarTerminal,
       StringTerminal
       > {};

     struct VariableTypeMember : boost::proto::member<
       VariableNameSpecifier,
       TypeTerminal
       > {};

     struct VariableRule : boost::proto::subscript<
       VariableTypeMember,
       TypeAccessRule
       > {};

     //
     // Functions:
     //
     // function["name"].type[int_(32)(float_(64), int_(32))] [
     //   Body
     // ];

     typedef Wrapper<boost::proto::terminal<keyword::function>::type> FunctionTerminal;

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

     /// This is the rule to match function symbols.  It matches
     /// function["name"].type["name"|type][body].
     typedef boost::proto::subscript<
       boost::proto::subscript<
	 boost::proto::member<
	   boost::proto::subscript<
	     FunctionTerminal,
	     StringTerminal
	     >,
	   TypeTerminal
	   >,
	 FunctionTypeAccessRule
	 >,
       VariableStatementList
       > FunctionRule;

  // Builder syntax
  //
  // Modules:
  //
  // module["name"] [
  //   Body
  // ];

     /// Define a terminal type for module symbols.
  typedef Wrapper<boost::proto::terminal<keyword::module>::type> ModuleTerminal;

  /// Define a rule to match a function, variable or type.
  typedef boost::proto::or_<
    FunctionRule,
    VariableRule,
    TypeRule
    > FunctionVariableType;

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
     typedef boost::proto::subscript<
       boost::proto::subscript<
	 ModuleTerminal,
	 StringTerminal
	 >,
       FunctionVariableTypeList
       > ModuleRule;
   }
}

#endif
