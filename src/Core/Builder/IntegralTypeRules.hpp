#ifndef mirv_Core_Builder_IntegralTypeRules_hpp
#define mirv_Core_Builder_IntegralTypeRules_hpp

#include <mirv/Core/Builder/IntegralTypeRulesFwd.hpp>
#include <mirv/Core/Builder/SymbolTerminals.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     typedef Wrapper<boost::proto::terminal<keyword::int_>::type> IntTerminal;

     /// This is the rule to match int type symbols.  It matches
     /// int_(int)
     struct IntRule : boost::proto::function<
       IntTerminal,
       IntegerTerminal
       > {};
   }
}

#endif
