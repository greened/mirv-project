#ifndef mirv_Core_Builder_IfRules_hpp
#define mirv_Core_Builder_IfRules_hpp

#include <mirv/Core/Builder/IfTerminals.hpp>
#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to match if statements.  It matches
     /// if_(e1)[e2].
     typedef boost::proto::subscript<
       boost::proto::function<
	 IfTerminal,
	 ConstructExpressionGrammar
	 >,
       ConstructStatementGrammar
       > IfRule;
   }
}

#endif
