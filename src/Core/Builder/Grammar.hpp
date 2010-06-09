#ifndef mirv_Core_Builder_Grammar_hpp
#define mirv_Core_Builder_Grammar_hpp

#include <mirv/Core/Builder/GrammarFwd.hpp>

#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Builder/StatementGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This aggregates all builder grammar rules and actions.  It
    /// serves as the grammar for building the whole IR.
    struct ConstructGrammar
      : boost::proto::or_<
      ConstructSymbolGrammar,
      ConstructExpressionGrammar,
      ConstructStatementGrammar> {};
  }
}

#endif
