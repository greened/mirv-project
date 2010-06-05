#ifndef mirv_Core_Builder_VoidTypeRules_hpp
#define mirv_Core_Builder_VoidTypeRules_hpp

#include <mirv/Core/Builder/Keywords.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/IR/Variable.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to match void types.
     typedef Wrapper<boost::proto::terminal<keyword::void_>::type> VoidTerminal;
   }
}

#endif
