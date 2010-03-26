#ifndef mirv_Core_Builder_Symbol_hpp
#define mirv_Core_Builder_Symbol_hpp

#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/DomainFwd.hpp>
#include <mirv/Core/Builder/GrammarFwd.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/IR/Variable.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     namespace keyword {
       /// This is a proto tag to build a terminal symbol to kick off
       /// module construction.
       struct module {
	 friend std::ostream& operator<<(std::ostream& sout, module) {
	   return sout << "module";
	 }
       };
       /// This is a proto tag to build a terminal symbol to kick off
       /// function construction.
       struct function {
	 friend std::ostream& operator<<(std::ostream& sout, function) {
	   return sout << "function";
	 }
       };
       /// This is a proto tag to build a terminal symbol to kick off
       /// variable construction.
       struct var {
	 friend std::ostream& operator<<(std::ostream& sout, var) {
	   return sout << "var";
	 }
       };
       /// This is a proto tag to build a terminal symbol to kick off
       /// type lookup.
       struct type {
	 friend std::ostream& operator<<(std::ostream& sout, type) {
	   return sout << "type";
	 }
       };
       /// This is a proto tag to build a terminal symbol to kick off
       /// struct type construction.
       struct struct_ {
	 friend std::ostream& operator<<(std::ostream& sout, struct_) {
	   return sout << "struct_";
	 }
       };
       /// This is a proto tag to build a terminal symbol to kick off
       /// integral type construction.
       struct int_ {
	 friend std::ostream& operator<<(std::ostream& sout, int_) {
	   return sout << "int_";
	 }
       };
       /// This is a proto tag to build a terminal symbol to kick off
       /// floating point type construction.
       struct float_ {
	 friend std::ostream& operator<<(std::ostream& sout, float_) {
	   return sout << "float_";
	 }
       };
       /// This is a proto tag to build a terminal symbol to kick off
       /// type construction when we need a way to specify no type.
       /// For example, conmstructing a function type with no return
       /// value.
       struct void_ {
	 friend std::ostream& operator<<(std::ostream& sout, void_) {
	   return sout << "void_";
	 }
       };
     }

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
     struct TypeList;
     typedef boost::proto::comma<
       TypeList,
       TypeRule
       > TypeListListPart;

     struct TypeList :
       boost::proto::or_<
       TypeRule,
       TypeListListPart
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
       TypeList
       > FunctionTypeWithArgsRule;

     typedef boost::proto::function<
       boost::proto::or_<
	 VoidTerminal,
	 TypeRule
	 >
       > FunctionTypeWithoutArgsRule;

     typedef boost::proto::or_<
       FunctionTypeWithArgsRule,
       FunctionTypeWithoutArgsRule
       > FunctionTypeRule;

     /// This is the rule to  match type symbols.  It matches
     /// IntType|FloatType|StructType|FunctionType
     struct TypeRule :
       boost::proto::or_<
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
