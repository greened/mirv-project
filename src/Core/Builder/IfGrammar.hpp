#ifndef mirv_Core_Builder_IfGrammar_hpp
#define mirv_Core_Builder_IfGrammar_hpp

#include <mirv/Core/Builder/IfGrammarFwd.hpp>

#include <mirv/Core/Builder/IfRules.hpp>
#include <mirv/Core/Builder/StatementTransforms.hpp>
#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Builder/StatementGrammarFwd.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for if statements.
    struct IfBuilder : boost::proto::when<
      IfRule,
      IfTransform(boost::proto::_data,
                  ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
                  PopScope(ConstructStatementGrammar(boost::proto::_right,
                                                     boost::proto::_state,
                                                     PushScope(boost::proto::_data)),
                           boost::proto::_data))
      > {};
  }
}

#endif
