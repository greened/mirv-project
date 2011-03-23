#ifndef mirv_Core_Builder_StructTypeRules_hpp
#define mirv_Core_Builder_StructTypeRules_hpp

#include <mirv/Core/Builder/TypeRulesFwd.hpp>
#include <mirv/Core/Builder/SymbolTerminals.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// Define a rule for a list of types.
     typedef Wrapper<boost::proto::terminal<keyword::struct_>::type> StructTerminal;

     /// This is the rule to match struct type symbols.  It matches
     /// struct_["name"][body]
     struct StructTypeDefRule : boost::proto::subscript<
       boost::proto::subscript<
	 StructTerminal,
	 StringTerminal
	 >,
       TypeList
       > {};

     struct StructTypeDeclRule : boost::proto::subscript<
       StructTerminal,
       StringTerminal
       > {};

     struct StructTypeRule : boost::proto::or_<
       StructTypeDefRule,
       StructTypeDeclRule
       > {};
   }
}

#endif
