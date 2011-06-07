#ifndef mirv_Core_Builder_VariableRules_hpp
#define mirv_Core_Builder_VariableRules_hpp

#include <mirv/Core/Builder/VariableRulesFwd.hpp>

#include <mirv/Core/Builder/TypeLookupRules.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     // Variables:
     //
     // var["name"].type[int_(32)];
     //

     /// This is the rule to match variable symbols.  It matches
     /// var["name"].type["name"|type].
     struct VariableNameSpecifier : boost::proto::subscript<
       VarTerminal,
       StringTerminal
       > {};

     struct VariableTypeMember : boost::proto::member<
       VariableNameSpecifier,
       TypeTerminal
       > {};

     /// Specify the rule for matching variable declarations.
     /// var["name"].type["name"|type]
     struct VariableRule : boost::proto::subscript<
       VariableTypeMember,
       TypeAccessRule
       > {};
   }
}

#endif
