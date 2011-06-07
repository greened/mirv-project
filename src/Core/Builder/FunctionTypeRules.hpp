#ifndef mirv_Core_Builder_FunctionTypeRules_hpp
#define mirv_Core_Builder_FunctionTypeRules_hpp

#include <mirv/Core/Builder/FunctionTypeRulesFwd.hpp>

#include <mirv/Core/Builder/TypeRules.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     typedef Wrapper<boost::proto::terminal<keyword::vararg>::type> VarargTerminal;

     // TODO: More strictly enforce that vararg should be at the end.
     /// This is the rule to match a function type argument type.
     /// ArgType -> Type | vararg
     struct ArgTypeRule : boost::proto::or_<
       TypeRule,
       VarargTerminal
       > {};

     /// This is therule to match function type symbols.  It matches
     /// Type|void_(TypeList)
     struct FunctionTypeRule : boost::proto::function<
       boost::proto::or_<
	 VoidTerminal,
	 ArgTypeRule
	 >,
       boost::proto::vararg<ArgTypeRule>
       > {};

     // typedef boost::proto::function<
     //   boost::proto::or_<
     //     VoidTerminal,
     //     TypeRule
     //     >
     //   > FunctionTypeWithoutArgsRule;

     // typedef boost::proto::or_<
     //   FunctionTypeWithArgsRule,
     //   FunctionTypeWithoutArgsRule
     //   > FunctionTypeRule;
   }
}

#endif
