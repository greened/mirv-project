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

     /// This is the rule to match struct type symbols for one-member
     /// structs.  It matches struct_["name"][type]
     struct StructSingleTypeDefRule : boost::proto::subscript<
       boost::proto::subscript<
	 StructTerminal,
	 StringTerminal
	 >,
       TypeRule
       > {};

     /// This is the rule to match struct type symbols.  It matches
     /// struct_["name"][body]
     struct StructMultiTypeDefRule : boost::proto::subscript<
       boost::proto::subscript<
	 StructTerminal,
	 StringTerminal
	 >,
       StrictTypeList
       > {};

     /// This is the rule for struct type declarations.
     /// StructTypeDecl -> struct_[string]
     struct StructTypeDeclRule : boost::proto::subscript<
       StructTerminal,
       StringTerminal
       > {};

     /// This is a rule to match struct types.
     /// StructType -> StructTypeDef | StructTypeDecl
     struct StructTypeRule : boost::proto::or_<
       StructSingleTypeDefRule,
       StructMultiTypeDefRule,
       StructTypeDeclRule
       > {};
   }
}

#endif
