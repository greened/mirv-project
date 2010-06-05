#ifndef mirv_Core_Builder_TypeGrammar_hpp
#define mirv_Core_Builder_TypeGrammar_hpp

#include <mirv/Core/Builder/IntegralTypeGrammar.hpp>
#include <mirv/Core/Builder/FloatingTypeGrammar.hpp>
#include <mirv/Core/Builder/FunctionTypeGrammar.hpp>
#include <mirv/Core/Builder/StructTypeGrammar.hpp>
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
    /// This is the grammar to construct types.
    struct TypeBuilder : boost::proto::or_<
      IntBuilder,
      FloatBuilder,
      FunctionTypeBuilder,
      StructTypeBuilder
      > {};
  }
}

#endif
