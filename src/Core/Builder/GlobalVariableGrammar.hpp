#ifndef mirv_Core_Builder_GlobalVariableGrammar_hpp
#define mirv_Core_Builder_GlobalVariableGrammar_hpp

#include <mirv/Core/Builder/GlobalVariableRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/Builder/TypeLookupGrammar.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for variable symbols.
    struct GlobalVariableBuilder : boost::proto::_or<
      boost::proto::when<
        GlobalVariableDecl,
        BinaryConstructSymbol<
          Symbol<GlobalVariable> >(
            boost::proto::_data,
            // Variable name
            boost::proto::_value(boost::proto::_right(
                                   boost::proto::_left(boost::proto::_left))),
            // Variable type
            TypeAccessBuilder(boost::proto::_right))
        >,
          boost::proto::when<
            GlobalVariableDeclWithInit,
            TernaryConstructSymbol<
              Symbol<GlobalVariable> >(
                boost::proto::_data,
                // Variable name
                boost::proto::_value(boost::proto::_right(
                                       boost::proto::_left(boost::proto::_left(boost::proto::_left)))),
                // Variable type
                TypeAccessBuilder(boost::proto::_right(boost::proto::_left)),
                // Initializer
                ConstantBuilder(boost::proto::_right))
            >
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
