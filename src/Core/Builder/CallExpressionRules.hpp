#ifndef mirv_Core_Builder_CallExpressionRules_hpp
#define mirv_Core_Builder_CallExpressionRules_hpp

#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
      // Nary expressions
     struct CallRule : boost::proto::function<ConstructExpressionGrammar, 
       boost::proto::vararg<ConstructExpressionGrammar>
       > {};
   }
}

#endif
