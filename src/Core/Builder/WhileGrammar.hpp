#ifndef mirv_Core_Builder_WhileGrammar_hpp
#define mirv_Core_Builder_WhileGrammar_hpp

#include <mirv/Core/Builder/WhileRules.hpp>

namespace mirv {
  namespace Builder {
    struct WhileBuilder : boost::proto::when<
      WhileRule,
      ConstructBinary<
        Statement<IfThen>,
        ptr<Expression<Base> >::type,
        ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
                                     ConstructBinary<
                                     Statement<DoWhile>,
                                     ptr<Expression<Base> >::type,
                                     ptr<Statement<Base> >::type>(ConstructExpressionGrammar(boost::proto::_right(boost::proto::_left)),
                                                                  ConstructStatementGrammar(boost::proto::_right)))
      > {};
  }
}

#endif