#ifndef mirv_Core_Builder_CallStatementGrammar_hpp
#define mirv_Core_Builder_CallStatementGrammar_hpp

#include <mirv/Core/Builder/ExpressionGrammarFwd.hpp>

#include <mirv/Core/Builder/CallExpressionRules.hpp>
#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/IR/Function.hpp>

#include <boost/proto/proto.hpp>
#include <boost/fusion/include/pop_front.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for function call statements.
    struct CallStatementBuilder :  boost::proto::when<
      CallRule,
      ConstructNary<
        Statement<Call>
        >(boost::proto::_data,
          ConstructExpressionGrammar(boost::proto::_left),
          boost::proto::_expr)
      > {};
  }
}

#endif
