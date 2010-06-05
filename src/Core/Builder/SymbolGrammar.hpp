#ifndef mirv_Core_Builder_SymbolGrammar_hpp
#define mirv_Core_Builder_SymbolGrammar_hpp

#include <mirv/Core/Builder/ModuleGrammarFwd.hpp>
#include <mirv/Core/Builder/FunctionGrammarFwd.hpp>
#include <mirv/Core/Builder/VariableGrammarFwd.hpp>
#include <mirv/Core/Builder/TypeGrammarFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This aggregates all of the symbol rules.  It serves as the
    /// grammar for all symbols.
    struct ConstructSymbolGrammar : boost::proto::or_<
      ModuleBuilder,
      FunctionBuilder,
      VariableBuilder,
      TypeBuilder
      > {};
  }
}

#endif
