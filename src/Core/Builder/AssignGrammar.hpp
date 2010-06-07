#ifndef mirv_Core_Builder_AssignGrammar_hpp
#define mirv_Core_Builder_AssignGrammar_hpp

#include <mirv/Core/Builder/AssignRules.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for assignment statements.
    struct AssignBuilder : boost::proto::when<
      AssignRule,
      ConstructBinary<Statement<Assignment> >(ConstructExpressionGrammar(boost::proto::_left),
                                              ConstructExpressionGrammar(boost::proto::_right))> {};
  }
}

#endif