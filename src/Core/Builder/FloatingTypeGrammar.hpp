#ifndef mirv_Core_Builder_FloatingTypeGrammar_hpp
#define mirv_Core_Builder_FloatingTypeGrammar_hpp

#include <mirv/Core/Builder/FloatingTypeRules.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for floating point types.
    struct FloatBuilder : boost::proto::when<
      FloatRule,
      // LookupAndAddSymbol<Type>(
      //   boost::proto::_data,
        UnaryConstructSymbol<FloatingPointType, ModuleScope>(
          boost::proto::_data,
          boost::proto::_value(boost::proto::_right))
          // )
      > {};

    namespace {
      /// A float_ "operator."  This is a protoized object that
      /// implements the function operator to construct float types.
      const FloatTerminal float_ = {{}};
    }
  }
}

#endif
