#ifndef mirv_Core_Builder_DoWhileRules_hpp
#define mirv_Core_Builder_DoWhileRules_hpp

#include <mirv/Core/Builder/DoWhileRulesFwd.hpp>

#include <mirv/Core/Builder/LoopTerminals.hpp>
#include <mirv/Core/Builder/ExpressionRules.hpp>
#include <mirv/Core/Builder/StatementRules.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/Builder/Domain.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to match do-while statements.  It matches
     /// do_[e1].while_(e2).
     struct DoWhileRule : boost::proto::function<
       boost::proto::member<
	 boost::proto::subscript<
	   DoTerminal,
	   StatementRule
	   >,
	 WhileTerminal
	 >,
       ExpressionRule
       > {};
   }
}

#endif
