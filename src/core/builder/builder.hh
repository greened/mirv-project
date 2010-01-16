#ifndef mirv_core_builder_builder_hh
#define mirv_core_builder_builder_hh

// Order is important here.  In particular, domain.hh must be included
// after grammar.hh.
#include <mirv/core/builder/expression.hh>
#include <mirv/core/builder/statement.hh>
#include <mirv/core/builder/grammar.hh>
#include <mirv/core/builder/domain.hh>
#include <mirv/core/builder/wrapper.hh>
#include <mirv/core/builder/make.hh>
#include <mirv/core/builder/translate.hh>

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
