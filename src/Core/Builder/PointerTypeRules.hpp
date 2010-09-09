#ifndef mirv_Core_Builder_PointerTypeRules_hpp
#define mirv_Core_Builder_PointerTypeRules_hpp

#include <mirv/Core/Builder/PointerTypeRulesFwd.hpp>
#include <mirv/Core/Builder/TypeGrammarFwd.hpp>

#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to match pointer type symbols.  It matches
     /// *<type>
     struct PointerTypeRule : boost::proto::dereference<
       TypeBuilder
       > {};
   }
}

#endif
