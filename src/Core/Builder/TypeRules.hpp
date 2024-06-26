#ifndef mirv_Core_Builder_TypeRules_hpp
#define mirv_Core_Builder_TypeRules_hpp

#include <mirv/Core/Builder/TypeRulesFwd.hpp>

#include <mirv/Core/Builder/IntegralTypeRulesFwd.hpp>
#include <mirv/Core/Builder/FloatingTypeRulesFwd.hpp>
#include <mirv/Core/Builder/VoidTypeRules.hpp>
#include <mirv/Core/Builder/StructTypeRulesFwd.hpp>
#include <mirv/Core/Builder/FunctionTypeRulesFwd.hpp>
#include <mirv/Core/Builder/PointerTypeRulesFwd.hpp>
#include <mirv/Core/Builder/ArrayTypeRulesFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// Define a rule for a list of types.
     struct TypeRule;
     struct TypeList;

     /// This is a rule to match a list of types.
     /// StructTypeList -> TypeList, Type
     struct StrictTypeList : boost::proto::comma<
       TypeList,
       TypeRule
       > {};

     /// Define a rule for a list of type.
     /// TypeList: type | (TypeList type)
     struct TypeList : boost::proto::or_<
       TypeRule,
       StrictTypeList
       > {};

     /// IntType|FloatType|StructType|FunctionType
     struct TypeRule : boost::proto::or_<
       IntRule,
       FloatRule,
       StructTypeRule,
       FunctionTypeRule,
       PointerTypeRule,
       ArrayTypeRule
       > {};

    /// This is the rule to lookup types.  It is almost the same as
    /// the construct rule except for struct types it assumes the
    /// struct already exists.  This lets the rule avoid creating and
    /// returning placeholders instead of the real type.
     struct TypeLookupRule : boost::proto::or_<
       IntRule,
       FloatRule,
       StructTypeDeclRule,
       FunctionTypeRule,
       PointerTypeRule,
       ArrayTypeRule
       > {};
   }
}

#endif
