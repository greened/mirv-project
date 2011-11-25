#ifndef mirv_Core_Builder_AssignGrammar_hpp
#define mirv_Core_Builder_AssignGrammar_hpp

#include <mirv/Core/Builder/AssignRules.hpp>
#include <mirv/Core/Builder/StatementTransforms.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for assignment statements.
    struct AssignBuilder : boost::proto::when<
      AssignRule,
      AssignTransform(
        boost::proto::_data,
        ConstructExpressionGrammar(boost::proto::_left),
        ConstructExpressionGrammar(boost::proto::_right))
      > {};
  }
}

#endif
