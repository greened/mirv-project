#ifndef mirv_Core_Builder_Builder_hpp
#define mirv_Core_Builder_Builder_hpp

// Order is important here.  In particular, Domain.hpp must be included
// after Grammar.hpp.
#include <mirv/Core/Builder/Symbol.hpp>
#include <mirv/Core/Builder/Expression.hpp>
#include <mirv/Core/Builder/Statement.hpp>
#include <mirv/Core/Builder/Grammar.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
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

      /// A function "operator."  This is a protoized object that
      /// implements the subscript operator to kick off function
      /// symbol generation.
      const FunctionTerminal function = {{}};

      /// A var "operator."  This is a protoized object that
      /// implements the subscript operator to kick off variable
      /// symbol generation.
      const VarTerminal var = {{}};

      /// A void_ "operator."  This is a protoized object that
      /// implements the function operator to construct function types
      /// with no return value.
      const VoidTerminal void_ = {{}};

      /// An int_ "operator."  This is a protoized object that
      /// implements the function operator to construct integer types.
      const IntTerminal int_ = {{}};
    }
  }
}

#endif
