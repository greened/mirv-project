#ifndef mirv_core_builder_Builder_hpp
#define mirv_core_builder_Builder_hpp

// Order is important here.  In particular, Domain.hpp must be included
// after Grammar.hpp.
#include <mirv/core/builder/Expression.hpp>
#include <mirv/core/builder/Statement.hpp>
#include <mirv/core/builder/Grammar.hpp>
#include <mirv/core/builder/Domain.hpp>
#include <mirv/core/builder/Wrapper.hpp>
#include <mirv/core/builder/Make.hpp>
#include <mirv/core/builder/Translate.hpp>

namespace mirv {
  namespace Builder {
    namespace {
      /// An if_ "operator."  This is a protoized object that implements
      /// the subscript operator to kick off if statement generation.
       const IfTerminal if_ = {{}};
    }

    namespace {
      /// A while_ "operator."  This is a protoized object that
      /// implements the subscript operator to kick off dowhile
      /// statement generation.
      const WhileTerminal while_ = {{}};
    }

    namespace {
      /// A do_ "operator."  This is a protoized object that
      /// implements the subscript operator to kick off dowhile
      /// statement generation.
      const DoTerminal do_ = {{}};
    }
  }
}

#endif
