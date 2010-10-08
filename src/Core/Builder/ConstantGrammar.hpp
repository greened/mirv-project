#ifndef mirv_Core_Builder_ConstantGrammar_hpp
#define mirv_Core_Builder_ConstantGrammar_hpp

#include <mirv/Core/Builder/ConstantRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/Builder/TypeLookupGrammar.hpp>
#include <mirv/Core/IR/Variable.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      struct IntegralTypeName {
        typedef std::string result_type;

        result_type operator()(size_t bitsize) {
          return "int" + boost::lexical_cast<std::string>(bitsize);
        }
      };
 
      struct FloatingTypeName {
        typedef std::string result_type;

        result_type operator()(size_t bitsize) {
          return "float"
            + boost::lexical_cast<std::string>(bitsize);
        }
      };
    }

    /// This is the grammar for constant symbols.
    struct ConstantBuilder : boost::proto::or_<
      boost::proto::when<
        IntegralConstantRule,
        ConstructConstantSymbol<detail::IntegralTypeName>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)
            >,
      boost::proto::when<
        FloatingConstantRule,
        ConstructConstantSymbol<detail::FloatingTypeName>(
            boost::proto::_data,
            // Constant expression
            boost::proto::_expr)
            >
      > {};
  }
}

#endif
