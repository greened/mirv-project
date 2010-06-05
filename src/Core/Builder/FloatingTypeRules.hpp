#ifndef mirv_Core_Builder_FloatingTypeRules_hpp
#define mirv_Core_Builder_FloatingTypeRules_hpp

#include <mirv/Core/Builder/FloatingTypeRulesFwd.hpp>

#include <mirv/Core/Builder/Keywords.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     typedef Wrapper<boost::proto::terminal<keyword::float_>::type> FloatTerminal;

     /// This is the rule to match float type symbols.  It matches
     /// float_(float)
     struct FloatRule : boost::proto::function<
       FloatTerminal,
       IntegerTerminal
       > {};
   }
}

#endif
