#ifndef mirv_Core_Builder_DoWhileGrammar_hpp
#define mirv_Core_Builder_DoWhileGrammar_hpp

#include <mirv/Core/Builder/DoWhileGrammarFwd.hpp>

#include <mirv/Core/Builder/DoWhileRules.hpp>

namespace mirv {
  namespace Builder {
    struct DoWhileBuilder : boost::proto::when<
      DoWhileRule,
      ClearPendingStatements<Statement<DoWhile> >(
        boost::proto::_data,
        ConstructBinary<
          Statement<DoWhile>,
          ptr<Expression<Base> >::type,
        ptr<Statement<Base> >::type>(boost::proto::_data,
                                     ConstructExpressionGrammar(boost::proto::_right),
                                     ConstructStatementGrammar(boost::proto::_right(boost::proto::_left(boost::proto::_left)))))
      > {};
  }
}

#endif
