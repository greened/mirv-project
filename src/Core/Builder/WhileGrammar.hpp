#ifndef mirv_Core_Builder_WhileGrammar_hpp
#define mirv_Core_Builder_WhileGrammar_hpp

#include <mirv/Core/Builder/WhileGrammarFwd.hpp>

#include <mirv/Core/Builder/WhileRules.hpp>
#include <mirv/Core/Builder/StatementTransforms.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar to construct do-while IR statements from a
    /// while statement.
    struct WhileBuilder : boost::proto::when<
      WhileRule,
      WhileTransform(boost::proto::_data,
                     ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
                     ConstructStatementGrammar(boost::proto::_right))
      > {};
  }
}

#endif
