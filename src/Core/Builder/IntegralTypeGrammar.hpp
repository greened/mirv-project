#ifndef mirv_Core_Builder_IntegralTypeGrammar_hpp
#define mirv_Core_Builder_IntegralTypeGrammar_hpp

#include <mirv/Core/Builder/IntegralTypeRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/IR/IntegralTypeFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for integral types.
    struct IntBuilder : boost::proto::when<
      IntRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        UnaryConstructSymbol<Symbol<Type<Integral> > >(
          boost::proto::_data,
          boost::proto::_value(boost::proto::_right)))
      > {};

    namespace {
      /// An int_ "operator."  This is a protoized object that
      /// implements the function operator to construct integer types.
      const IntTerminal int_ = {{}};
    }
  }
}

#endif
