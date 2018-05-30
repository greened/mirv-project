#ifndef mirv_Core_Builder_AssignRules_hpp
#define mirv_Core_Builder_AssignRules_hpp

#include <mirv/Core/Builder/ExpressionRules.hpp>
#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to match assignment statements.
     typedef boost::proto::assign<ExpressionRule,
                                  ExpressionRule> AssignRule;
   }
}

#endif
