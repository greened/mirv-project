#ifndef mirv_Core_Builder_StatementGrammar_hpp
#define mirv_Core_Builder_StatementGrammar_hpp

#include <mirv/Core/Builder/AssignGrammar.hpp>
#include <mirv/Core/Builder/SequenceGrammar.hpp>
#include <mirv/Core/Builder/IfGrammar.hpp>
#include <mirv/Core/Builder/IfElseGrammar.hpp>
#include <mirv/Core/Builder/WhileGrammar.hpp>
#include <mirv/Core/Builder/DoWhileGrammar.hpp>

namespace mirv {
  namespace Builder {
    /// This aggregates all of the statement rules.  It serves as the
    /// grammar for all statements.
    struct ConstructStatementGrammar
      : boost::proto::or_<
      AssignBuilder,
      SequenceBuilder,
      IfBuilder,
      IfElseBuilder,
      WhileBuilder,
      DoWhileBuilder
      > {};
  }
}

#endif
