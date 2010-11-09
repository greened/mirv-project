#ifndef mirv_Core_Builder_IfGrammar_hpp
#define mirv_Core_Builder_IfGrammar_hpp

#include <mirv/Core/Builder/IfGrammarFwd.hpp>

#include <mirv/Core/Builder/IfRules.hpp>
#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Builder/StatementGrammarFwd.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for if statements.
    struct IfBuilder : boost::proto::when<
      IfRule,
      ClearPendingStatements<Statement<IfThen> >(
        boost::proto::_data,
        ConstructBinary<
          Statement<IfThen>,
          ptr<Expression<Base> >::type,
          ptr<Statement<Base> >::type>(boost::proto::_data,
                                       ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
                                       ConstructStatementGrammar(boost::proto::_right)))
      > {};
  }
}

#endif
