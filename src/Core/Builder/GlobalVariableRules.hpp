#ifndef mirv_Core_Builder_GlobalVariableRules_hpp
#define mirv_Core_Builder_GlobalVariableRules_hpp

#include <mirv/Core/Builder/GlobalVariableRulesFwd.hpp>
#include <mirv/Core/Builder/ConstantGrammarFwd.hpp>

#include <mirv/Core/Builder/TypeLookupRules.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     // GlobalVariables:
     //
     // var["name"].type[int_(32)] [= <init>]
     //

     typedef Wrapper<boost::proto::terminal<keyword::var>::type> VarTerminal;

     /// This is the rule to match variable symbols.  It matches
     /// var["name"].type["name"|type] [= <init>].
     struct GlobalVariableNameSpecifier : boost::proto::subscript<
       VarTerminal,
       StringTerminal
       > {};

     struct GlobalVariableTypeMember : boost::proto::member<
       VariableNameSpecifier,
       TypeTerminal
       > {};

     struct GlobalVariableDecl : boost::proto::subscript<
       VariableTypeMember,
       TypeAccessRule
       > {};

     struct GlobalVariableDeclWithInit : boost::proto::assign<
       GlobalVariableDecl,
       ConstantBuilder
       > {};

     struct GlobalVariableRule : boost::proto::_or<
       GlobalVariableDecl,
       GlobalVariableDeclWithInit
       > {};
   }
}

#endif
