#ifndef mirv_Core_Builder_Statement_hpp
#define mirv_Core_Builder_Statement_hpp

#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/Builder/DomainFwd.hpp>
#include <mirv/Core/Builder/GrammarFwd.hpp>
#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to match assignment statements.
     typedef boost::proto::assign<ConstructExpressionGrammar, ConstructExpressionGrammar> Assign;
     /// This is the rule to match a sequence of statements.  The
     /// comma serves the same purpose as the semicolon in C-like
     /// languages.
     typedef boost::proto::comma<ConstructStatementGrammar, ConstructStatementGrammar> Comma;

     namespace keyword {
       /// This is a proto tag to build a terminal symbol to kick off
       /// if-then and if-else statement construction.
       struct if_ {
	 friend std::ostream& operator<<(std::ostream& sout, if_) {
	   return sout << "if_";
	 }
       };
       /// This is a proto tag to build a terminal symbol to kick off
       /// building of the else portion of an if-else statement.
       struct else_ { 
	 friend std::ostream& operator<<(std::ostream& sout, else_) {
	   return sout << "else_";
	 }
       };
       /// This is a proto tag to build a terminal symbol to kick off
       /// do-while statement construction.
       struct do_ {
	 friend std::ostream& operator<<(std::ostream& sout, do_) {
	   return sout << "do_";
	 }
       };
       /// This is a proto tag to build a terminal symbol to kick off
       /// do-while statement construction specified as a C-like while
       /// statement.  It also serves to kick off construction of the
       /// while portion of a do-while statement.
       struct while_ {
	 friend std::ostream& operator<<(std::ostream& sout, while_) {
	   return sout << "while_";
	 }
       };
     }

     /// Define a terminal type for if-then and if-else statements.
     typedef Wrapper<boost::proto::terminal<keyword::if_>::type> IfTerminal;

     /// This is the rule to match if statements.  It matches
     /// if_(e1)[e2].
     typedef boost::proto::subscript<
       boost::proto::function<
	 IfTerminal,
	 ConstructExpressionGrammar
	 >,
       ConstructStatementGrammar
       > IfRule;

     /// Define a terminal type for the else portion of if-else
     /// statements.
     typedef Wrapper<boost::proto::terminal<keyword::else_>::type> ElseTerminal;

     /// This is the rule to match if-else statements.  It matches
     /// if_(e1)[e2].else_[e3].
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

     /// Define a terminal type for the while keyword.
     typedef Wrapper<boost::proto::terminal<keyword::while_>::type> WhileTerminal;

     /// This is the rule to match while_ statements.  It matches
     /// while_(e1)[e2].
    typedef boost::proto::subscript<
       boost::proto::function<
	 WhileTerminal,
	 ConstructExpressionGrammar
	 >,
       ConstructStatementGrammar
       > WhileRule;

     /// Define a terminal type for the do keyword.
     typedef Wrapper<boost::proto::terminal<keyword::do_>::type> DoTerminal;

     /// This is the rule to match do-while statements.  It matches
     /// do_[e1].while_(e2).
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
