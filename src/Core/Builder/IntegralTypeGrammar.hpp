#ifndef mirv_Core_Builder_IntegralTypeGrammar_hpp
#define mirv_Core_Builder_IntegralTypeGrammar_hpp

#include <mirv/Core/Builder/IntegralTypeRules.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  class IntegerType;

  namespace Builder {
    /// This is the grammar for integer types.
    struct IntBuilder : boost::proto::when<
      IntRule,
      // LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
      //   boost::proto::_data,
      UnaryConstructSymbol<IntegerType, ModuleScope>(
          boost::proto::_data,
          boost::proto::_value(boost::proto::_right))
          // )
      > {};

    namespace {
      /// An int_ "operator."  This is a protoized object that
      /// implements the function operator to construct integer types.
      const IntTerminal int_ = {{}};
    }
  }
}

#endif
