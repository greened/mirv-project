#ifndef mirv_Core_Builder_VariableGrammar_hpp
#define mirv_Core_Builder_VariableGrammar_hpp

#include <mirv/Core/Builder/VariableRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/Builder/TypeLookupGrammar.hpp>
#include <mirv/Core/IR/Variable.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for variable symbols.
    struct VariableBuilder : boost::proto::when<
      VariableRule,
      BinaryConstructSymbol<
        Symbol<Variable> >(
          boost::proto::_data,
          // Variable name
          boost::proto::_value(boost::proto::_right(
                                 boost::proto::_left(boost::proto::_left))),
          // Variable type
          TypeAccessBuilder(boost::proto::_right),
          CurrentScope)
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
