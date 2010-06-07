#ifndef mirv_Core_Builder_AssignRules_hpp
#define mirv_Core_Builder_AssignRules_hpp

#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to match assignment statements.
     typedef boost::proto::assign<ConstructExpressionGrammar, ConstructExpressionGrammar> AssignRule;
   }
}

#endif
