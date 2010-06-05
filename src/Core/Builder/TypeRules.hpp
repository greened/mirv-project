#ifndef mirv_Core_Builder_TypeRules_hpp
#define mirv_Core_Builder_TypeRules_hpp

#include <mirv/Core/Builder/TypeRulesFwd.hpp>

#include <mirv/Core/Builder/IntegralTypeRulesFwd.hpp>
#include <mirv/Core/Builder/FloatingTypeRulesFwd.hpp>
#include <mirv/Core/Builder/VoidTypeRules.hpp>
#include <mirv/Core/Builder/StructTypeRulesFwd.hpp>
#include <mirv/Core/Builder/FunctionTypeRulesFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// Define a rule for a list of types.
     struct TypeRule;
     struct TypeList;

     struct StrictTypeList : boost::proto::comma<
       TypeList,
       TypeRule
       > {};

     struct TypeList : boost::proto::or_<
       TypeRule,
       StrictTypeList
       > {};

     /// IntType|FloatType|StructType|FunctionType
     struct TypeRule : boost::proto::or_<
       IntRule,
       FloatRule,
       StructTypeRule,
       FunctionTypeRule
       > {};
   }
}

#endif
