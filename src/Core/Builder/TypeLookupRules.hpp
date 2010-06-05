#ifndef mirv_Core_Builder_TypeLookupRules_hpp
#define mirv_Core_Builder_TypeLookupRules_hpp

#include <mirv/Core/Builder/TypeLookupRulesFwd.hpp>

#include <mirv/Core/Builder/SymbolTerminals.hpp>
#include <mirv/Core/Builder/TypeRules.hpp>
#include <mirv/Core/Builder/FunctionTypeRules.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to  match type symbols.  It matches
     /// Define a rule to access a type from a module.
     struct TypeAccessRule : boost::proto::or_<
       StringTerminal,
       TypeRule
       > {};

     /// Define a rule to access a function type from a module.
     struct FunctionTypeAccessRule : boost::proto::or_<
       StringTerminal,
       FunctionTypeRule
       > {};
   }
}

#endif
