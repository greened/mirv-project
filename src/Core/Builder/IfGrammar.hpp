#ifndef mirv_Core_Builder_IfGrammar_hpp
#define mirv_Core_Builder_IfGrammar_hpp

#include <mirv/Core/Builder/IfRules.hpp>
#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Builder/StatementGrammarFwd.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for if statements.
    struct IfBuilder : boost::proto::when<
      IfRule,
      ConstructBinary<
        Statement<IfThen>,
        ptr<Expression<Base> >::type,
        ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
                                     ConstructStatementGrammar(boost::proto::_right))
      > {};
  }
}

#endif
