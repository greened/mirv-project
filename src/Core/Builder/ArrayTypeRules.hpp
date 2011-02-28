#ifndef mirv_Core_Builder_ArrayTypeRules_hpp
#define mirv_Core_Builder_ArrayTypeRules_hpp

#include <mirv/Core/Builder/ArrayTypeRulesFwd.hpp>
#include <mirv/Core/Builder/TypeRulesFwd.hpp>
#include <mirv/Core/Builder/SymbolTerminals.hpp>

#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    struct TwoIntegerList : boost::proto::comma<
      IntegerTerminal,
      IntegerTerminal
      > {};

    struct IntegerList;

    struct RecursiveIntegerList : boost::proto::comma<
      IntegerList,
      IntegerTerminal
      > {};

    struct MoreIntegerList : boost::proto::or_<
      RecursiveIntegerList,
      IntegerTerminal
      > {};

    /// Production for a list of 3 or more integers.
    struct MultipleIntegerList : boost::proto::comma<
      TwoIntegerList,
      MoreIntegerList
      > {};

    struct IntegerList : boost::proto::or_<
      TwoIntegerList,
      MultipleIntegerList
      > {};

    struct MultiTypeSubscriptRule : boost::proto::subscript<
      TypeRule,
      IntegerList
      > {};

    struct TypeSubscriptList;

    struct StrictTypeSubscriptList : boost::proto::subscript<
      TypeSubscriptList,
      IntegerTerminal
      > {};

    struct TypeSubscriptRule : boost::proto::subscript<
      TypeRule,
      IntegerTerminal
      > {};

    struct TypeSubscriptList : boost::proto::or_<
      StrictTypeSubscriptList,
      TypeSubscriptRule
      > {};

    struct TypeDimensionList : boost::proto::or_<
      TypeSubscriptList,
      MultiTypeSubscriptRule
      > {};

    /// This is the rule to match array type symbols.  It matches
    /// <type>([int])+
    struct ArrayTypeRule : TypeDimensionList {};
  }
}

#endif

