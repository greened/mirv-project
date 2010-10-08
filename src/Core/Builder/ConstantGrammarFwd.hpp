#ifndef mirv_Core_Builder_ConstantGrammarFwd_hpp
#define mirv_Core_Builder_ConstantGrammarFwd_hpp

#include <mirv/Core/Builder/SymbolRules.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/proto/proto.hpp>
#include <boost/mpl/print.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/type_traits.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for constant symbols.
    struct ConstantBuilder;
  }
}

#endif
