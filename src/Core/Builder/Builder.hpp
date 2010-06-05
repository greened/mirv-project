#ifndef mirv_Core_Builder_Builder_hpp
#define mirv_Core_Builder_Builder_hpp

// Order is important here.  In particular, Domain.hpp must be included
// after Grammar.hpp.
#include <mirv/Core/Builder/SymbolTerminals.hpp>
#include <mirv/Core/Builder/ExpressionRules.hpp>
#include <mirv/Core/Builder/StatementRules.hpp>
#include <mirv/Core/Builder/Grammar.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/Builder/Translate.hpp>

namespace mirv {
  namespace Builder {
    namespace {
      /// An if_ "operator."  This is a protoized object that implements
      /// the subscript operator to kick off if statement generation.
       const IfTerminal if_ = {{}};

      /// A while_ "operator."  This is a protoized object that
      /// implements the subscript operator to kick off dowhile
      /// statement generation.
      const WhileTerminal while_ = {{}};

      /// A do_ "operator."  This is a protoized object that
      /// implements the subscript operator to kick off dowhile
      /// statement generation.
      const DoTerminal do_ = {{}};
    }
  }
}

#endif
