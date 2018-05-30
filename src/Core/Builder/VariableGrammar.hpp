#ifndef mirv_Core_Builder_VariableGrammar_hpp
#define mirv_Core_Builder_VariableGrammar_hpp

#include <mirv/Core/Builder/VariableRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/TypeLookupGrammar.hpp>
#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Producers.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for local variable symbols.
    struct VariableBuilder : boost::proto::when<
      VariableRule,
      AddAllocateStatement(
        boost::proto::_data,
        ConstructUnary<
          Allocate,
          Allocate::TypeHandle>(
            boost::proto::_data,
            // Type to allocate
            TypeAccessBuilder(boost::proto::_right)))
      > {};

    namespace {
      /// A var "operator."  This is a protoized object that
      /// implements the subscript operator to kick off variable
      /// symbol generation.
      const VarTerminal var = {{}};
    }
  }
}

#endif
