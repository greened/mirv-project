#ifndef mirv_Core_Builder_CallExpressionRules_hpp
#define mirv_Core_Builder_CallExpressionRules_hpp

#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     // Nary expressions
     /// This is a rule to match a call expression.
     /// Call -> Expression '(' Expression? (, Expression)* ')'
     struct FunctionCallRule : boost::proto::function<ConstructExpressionGrammar, 
       boost::proto::vararg<ConstructExpressionGrammar>
       > {};
   }
}

#endif
