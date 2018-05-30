#ifndef mirv_Core_Builder_IfElseGrammar_hpp
#define mirv_Core_Builder_IfElseGrammar_hpp

#include <mirv/Core/Builder/IfElseGrammarFwd.hpp>

#include <mirv/Core/Builder/IfElseRules.hpp>
#include <mirv/Core/Builder/StatementTransforms.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar to construct if-else IR statements.
    struct IfElseBuilder : boost::proto::when<
      IfElseRule,
      IfElseTransform(boost::proto::_data,
                      ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left(boost::proto::_left)))),
                      PopScope(ConstructStatementGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left)),
                                                         PushScope(boost::proto::_data)),
                               boost::proto::_data),
                      PopScope(ConstructStatementGrammar(boost::proto::_right,
                                                         PushScope(boost::proto::_data)),
                               boost::proto::_data))
      > {};
  }
}

#endif
