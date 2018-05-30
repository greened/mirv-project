#ifndef mirv_Core_Builder_SequenceGrammar_hpp
#define mirv_Core_Builder_SequenceGrammar_hpp

#include <mirv/Core/Builder/SequenceRules.hpp>
#include <mirv/Core/Builder/StatementTransforms.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for block statements.  The comma plays the
    /// role of the semicolon in C-like languages.
    struct SequenceBuilder : boost::proto::when<
      CommaRule,
      BlockTransform(boost::proto::_data,
                             ConstructStatementGrammar(boost::proto::_left),
                             ConstructStatementGrammar(boost::proto::_right))> {};
  }
}

#endif
