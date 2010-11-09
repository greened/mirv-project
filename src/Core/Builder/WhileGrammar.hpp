#ifndef mirv_Core_Builder_WhileGrammar_hpp
#define mirv_Core_Builder_WhileGrammar_hpp

#include <mirv/Core/Builder/WhileGrammarFwd.hpp>

#include <mirv/Core/Builder/WhileRules.hpp>

namespace mirv {
  namespace Builder {
    struct WhileBuilder : boost::proto::when<
      WhileRule,
      // Since there is not while statement, indicate a while by
      // specializing on the grammar rule.
      ClearPendingStatements<WhileRule> (
        boost::proto::_data,
        ConstructBinary<
          Statement<IfThen>,
          ptr<Expression<Base> >::type,
          ptr<Statement<Base> >::type>(boost::proto::_data,
                                       ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
                                       ConstructBinary<
                                       Statement<DoWhile>,
                                       ptr<Expression<Base> >::type,
                                       ptr<Statement<Base> >::type>(boost::proto::_data,
                                                                    ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
                                                                    ConstructStatementGrammar(boost::proto::_right))))
      > {};
  }
}

#endif
