#ifndef mirv_core_builder_statement_hh
#define mirv_core_builder_statement_hh

#include <mirv/core/builder/wrapper.hh>
#include <mirv/core/builder/domain_fwd.hh>
#include <mirv/core/builder/grammar_fwd.hh>
#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     typedef boost::proto::assign<ConstructExpressionGrammar, ConstructExpressionGrammar> Assign;
     typedef boost::proto::comma<ConstructStatementGrammar, ConstructStatementGrammar> Comma;

     namespace keyword {
       struct if_ {
	 friend std::ostream& operator<<(std::ostream& sout, if_) {
	   return sout << "if_";
	 }
       };
       struct else_ { 
	 friend std::ostream& operator<<(std::ostream& sout, else_) {
	   return sout << "else_";
	 }
       };
       struct do_ {
	 friend std::ostream& operator<<(std::ostream& sout, do_) {
	   return sout << "do_";
	 }
       };
       struct while_ {
	 friend std::ostream& operator<<(std::ostream& sout, while_) {
	   return sout << "while_";
	 }
       };
     }
     // Here is the grammar for if_ statements
     // matches if_(e1)[e2]
     typedef Wrapper<boost::proto::terminal<keyword::if_>::type> IfTerminal;

     typedef boost::proto::subscript<
       boost::proto::function<
	 IfTerminal,
	 ConstructExpressionGrammar
	 >,
       ConstructStatementGrammar
       > IfRule;

     typedef Wrapper<boost::proto::terminal<keyword::else_>::type> ElseTerminal;

     typedef boost::proto::subscript<
       boost::proto::member<
	 boost::proto::subscript<
	   boost::proto::function<
	     IfTerminal,
	     ConstructExpressionGrammar
	     >,
	   ConstructStatementGrammar
	   >,
	 ElseTerminal
	 >,
       ConstructStatementGrammar
       > IfElseRule;

     // Here is the grammar for while_ statements
     // matches while_(e1)[e2]

     typedef Wrapper<boost::proto::terminal<keyword::while_>::type> WhileTerminal;

     typedef boost::proto::subscript<
       boost::proto::function<
	 WhileTerminal,
	 ConstructExpressionGrammar
	 >,
       ConstructStatementGrammar
       > WhileRule;

     // Here is the grammar for do_ statements
     // A do_ "operator"
     typedef Wrapper<boost::proto::terminal<keyword::do_>::type> DoTerminal;

     typedef boost::proto::function<
       boost::proto::member<
	 boost::proto::subscript<
	   DoTerminal,
	   ConstructStatementGrammar
	   >,
	 WhileTerminal
	 >,
	   ConstructExpressionGrammar
	   > DoWhileRule;

      // TODO: Switch
   }
}

#endif
