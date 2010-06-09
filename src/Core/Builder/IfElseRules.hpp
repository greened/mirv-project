#ifndef mirv_Core_Builder_IfElseRules_hpp
#define mirv_Core_Builder_IfElseRules_hpp

#include <mirv/Core/Builder/IfTerminals.hpp>
#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Builder/StatementGrammar.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/Builder/Domain.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to match if-else statements.  It matches
     /// if_(e1)[e2].else_[e3].
     struct IfElseRule : boost::proto::subscript<
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
       > {};
   }
}

#endif
