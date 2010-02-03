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
       /// struct type construction.
       struct int_ {
	 friend std::ostream& operator<<(std::ostream& sout, int_) {
	   return sout << "int_";
	 }
       };
       /// This is a proto tag to build a terminal symbol to kick off
       /// struct type construction.
       struct float_ {
	 friend std::ostream& operator<<(std::ostream& sout, float_) {
	   return sout << "float_";
	 }
       };
     }

     /// A terminal for string constants.
     typedef Wrapper<boost::proto::terminal<std::string>::type> StringTerminal;
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

     /// This is the rule to match simple type symbols.  It matches
     /// int_(int)|float_(float)
     typedef boost::proto::or_<
       IntRule,
       FloatRule
       > SimpleTypeRule;

     /// Define a rule for a list of types.
     struct TypeRule;
     struct TypeList :
       boost::proto::or_<
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

     /// This is the rule to match type symbols.  It matches
     /// IntType|FloatType|StructType
     class TypeRule :
       boost::proto::or_<
       SimpleTypeRule,
       StructTypeRule
       > {};

     /// Define a rule to access a type from a module.
     typedef boost::proto::or_<
       StringTerminal,
       TypeRule
       > TypeAccessRule;

     // Variables:
     //
     // var["name"].type[int_(32)];
     //

     typedef Wrapper<boost::proto::terminal<keyword::var>::type> VarTerminal;
     typedef Wrapper<boost::proto::terminal<keyword::type>::type> TypeTerminal;

     /// This is the rule to match variable symbols.  It matches
     /// var["name"].type["name"|type].
     typedef boost::proto::subscript<
       boost::proto::member<
	 boost::proto::subscript<
	   VarTerminal,
	   StringTerminal
	   >,
	 TypeTerminal
	 >,
       TypeAccessRule
       > VariableRule;

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
  typedef boost::proto::or_<
    VariableRule,
    ConstructStatementGrammar
    > VariableOrStatement;

  /// Define a rule to match a list of variables and statements.
     struct VariableStatementList : public boost::proto::or_<
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
      TypeAccessRule
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

  typedef Wrapper<boost::proto::terminal<keyword::module>::type> ModuleTerminal;

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
