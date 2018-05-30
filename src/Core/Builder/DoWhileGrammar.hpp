#ifndef mirv_Core_Builder_DoWhileGrammar_hpp
#define mirv_Core_Builder_DoWhileGrammar_hpp

#include <mirv/Core/Builder/DoWhileGrammarFwd.hpp>

#include <mirv/Core/Builder/DoWhileRules.hpp>
#include <mirv/Core/Builder/StatementTransforms.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar to construction do-while IR statements.
    struct DoWhileBuilder : boost::proto::when<
      DoWhileRule,
      DoWhileTransform(
        boost::proto::_data,
        ConstructExpressionGrammar(boost::proto::_right),
        PopScope(ConstructStatementGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left)),
                                           PushScope(boost::proto::_data)),
                 boost::proto::_data))
      > {};
  }
}

#endif
