#ifndef mirv_Core_Builder_FunctionTypeRules_hpp
#define mirv_Core_Builder_FunctionTypeRules_hpp

#include <mirv/Core/Builder/FunctionTypeRulesFwd.hpp>

#include <mirv/Core/Builder/TypeRules.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is the rule to match function type symbols.  It matches
     /// Type|void_(TypeList)
     struct FunctionTypeRule : boost::proto::function<
       boost::proto::or_<
	 VoidTerminal,
	 TypeRule
	 >,
       boost::proto::vararg<TypeRule>
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
