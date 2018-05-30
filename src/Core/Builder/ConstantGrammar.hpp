#ifndef mirv_Core_Builder_ConstantGrammar_hpp
#define mirv_Core_Builder_ConstantGrammar_hpp

#include <mirv/Core/Builder/ConstantRules.hpp>
#include <mirv/Core/Builder/ConstantSymbolTransforms.hpp>

#include <boost/proto/proto.hpp>

#include <cstdint>

namespace mirv {
  namespace Builder {
    /// This is the grammar for constant symbols.
    struct ConstantBuilder : boost::proto::or_<
      boost::proto::when<
        IntegralConstantRule,
        ConstructConstantSymbol<detail::IntegerConstantGen>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)
            >,
      boost::proto::when<
        FloatingConstantRule,
        ConstructConstantSymbol<detail::FloatingPointConstantGen>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)
        >
      > {};

  /// Given a string constant, construct a reference to the address of
  /// the first character, to give C semantics.
    struct StringRefBuilder : boost::proto::when<
      StringConstantRule,
      detail::GetCStringReference(
        boost::proto::_data,
        AddStringConstant(
          boost::proto::_data,
          ConstructConstantSymbol<detail::StringConstantGen>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)))
      > {};
  }
}

#endif
