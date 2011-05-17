#ifndef mirv_Core_Builder_GlobalVariableGrammar_hpp
#define mirv_Core_Builder_GlobalVariableGrammar_hpp

#include <mirv/Core/Builder/GlobalVariableRules.hpp>
#include <mirv/Core/Builder/ExpressionGrammar.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/Builder/TypeLookupGrammar.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for variable symbols.
    struct GlobalVariableBuilder : boost::proto::or_<
      boost::proto::when<
        GlobalVariableDeclWithInit,
        TernaryConstructSymbol<
          Symbol<GlobalVariable>,
          ModuleScope>(
            boost::proto::_data,
            // Variable name
            boost::proto::_value(boost::proto::_right(
                                   boost::proto::_left(boost::proto::_left(boost::proto::_left)))),
            // Variable type
            TypeAccessBuilder(boost::proto::_right(boost::proto::_left)),
            // Initializer
            ConstantRefBuilder(boost::proto::_right))
        >,
      boost::proto::when<
        GlobalVariableDecl,
        BinaryConstructSymbol<
          Symbol<GlobalVariable>,
          ModuleScope>(
            boost::proto::_data,
            // Variable name
            boost::proto::_value(boost::proto::_right(
                                   boost::proto::_left(boost::proto::_left))),
            // Variable type
            TypeAccessBuilder(boost::proto::_right))
        >
      > {};
  }
}

#endif
