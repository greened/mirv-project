#ifndef mirv_Core_Builder_IfElseGrammar_hpp
#define mirv_Core_Builder_IfElseGrammar_hpp

#include <mirv/Core/Builder/IfElseGrammarFwd.hpp>

#include <mirv/Core/Builder/IfElseRules.hpp>

namespace mirv {
  namespace Builder {
    struct IfElseBuilder : boost::proto::when<
      IfElseRule,
      ClearPendingStatements(
        boost::proto::_data,
        ConstructTernary<
          Statement<IfElse>,
          ptr<Expression<Base> >::type,
          ptr<Statement<Base> >::type,
          ptr<Statement<Base> >::type>(boost::proto::_data,
                                       ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left(boost::proto::_left)))),
                                       ConstructStatementGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left))),
                                       ConstructStatementGrammar(boost::proto::_right)))
      > {};
  }
}

#endif
