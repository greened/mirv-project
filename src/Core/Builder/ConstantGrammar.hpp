#ifndef mirv_Core_Builder_ConstantGrammar_hpp
#define mirv_Core_Builder_ConstantGrammar_hpp

#include <mirv/Core/Builder/ConstantRules.hpp>
#include <mirv/Core/Builder/ConstantSymbolTransforms.hpp>

#include <boost/proto/proto.hpp>

#include <cstdint>

namespace mirv {
  namespace Builder {
    namespace detail {
      struct IntegralTypeGen {
        typedef ptr<Symbol<Type<TypeBase> > >::const_type result_type;
        typedef std::uint64_t value_type;

        result_type operator()(ptr<SymbolTable>::type symtab,
                               size_t bitsize);
      };
 
      struct FloatingTypeGen {
        typedef ptr<Symbol<Type<TypeBase> > >::const_type result_type;
        typedef double value_type;

        result_type operator()(ptr<SymbolTable>::type symtab,
                               size_t bitsize);
      };
    }

    /// This is the grammar for constant symbols.
    struct ConstantBuilder : boost::proto::or_<
      boost::proto::when<
        IntegralConstantRule,
        ConstructConstantSymbol<detail::IntegralTypeGen>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)
            >,
      boost::proto::when<
        FloatingConstantRule,
        ConstructConstantSymbol<detail::FloatingTypeGen>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)
            >
      > {};
  }
}

#endif
