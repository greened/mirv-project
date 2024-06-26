#ifndef mirv_Core_Builder_WhileRules_hpp
#define mirv_Core_Builder_WhileRules_hpp

#include <mirv/Core/Builder/LoopTerminals.hpp>
#include <mirv/Core/Builder/ExpressionRules.hpp>
#include <mirv/Core/Builder/StatementRules.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to match while_ statements.  It matches
     /// while_(e1)[e2].
     struct WhileRule : boost::proto::subscript<
       boost::proto::function<
	 WhileTerminal,
         ExpressionRule
	 >,
       StatementRule
       > {};
   }
}

#endif
