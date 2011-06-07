#ifndef mirv_Core_Builder_ArrayTypeRules_hpp
#define mirv_Core_Builder_ArrayTypeRules_hpp

#include <mirv/Core/Builder/ArrayTypeRulesFwd.hpp>
#include <mirv/Core/Builder/TypeRulesFwd.hpp>
#include <mirv/Core/Builder/SymbolTerminals.hpp>

#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is a rule to match a list of type integers.
    /// TwoIntegerList -> <int>, <int>
    struct TwoIntegerList : boost::proto::comma<
      IntegerTerminal,
      IntegerTerminal
      > {};

    struct IntegerList;

    /// This is a rule to match a list of integers.
    /// RecursiveIntegerList -> IntegerList, <int>
    struct RecursiveIntegerList : boost::proto::comma<
      IntegerList,
      IntegerTerminal
      > {};

    /// This is a rule to match a list of more than two integers.
    /// MoreIntegerList -> RecursiveIntegerList | <int>
    struct MoreIntegerList : boost::proto::or_<
      RecursiveIntegerList,
      IntegerTerminal
      > {};

    /// Production for a list of 3 or more integers.
    struct MultipleIntegerList : boost::proto::comma<
      TwoIntegerList,
      MoreIntegerList
      > {};

    /// Match a list of integers.
    /// int(, int)+
    struct IntegerList : boost::proto::or_<
      TwoIntegerList,
      MultipleIntegerList
      > {};

    /// Match a sequence of array dimensions.
    /// type[int,int,int,...]
    struct MultiTypeSubscriptRule : boost::proto::subscript<
      TypeRule,
      IntegerList
      > {};

    struct TypeSubscriptList;

    /// This is a rule to match a list of array dimensions.
    /// StructTypeSubscriptList -> TypeSubscriptList, <int>
    struct StrictTypeSubscriptList : boost::proto::subscript<
      TypeSubscriptList,
      IntegerTerminal
      > {};

    struct TypeSubscriptRule : boost::proto::subscript<
      TypeRule,
      IntegerTerminal
      > {};

    /// This is a rule to match a list of dimensions.
    /// TypeSubscriptList -> StrictTypeSubscriptList | TypeSubscript
    struct TypeSubscriptList : boost::proto::or_<
      StrictTypeSubscriptList,
      TypeSubscriptRule
      > {};

    /// This is the rule to match a list of type dimensions.
    /// TypeDimensionList -> TypeSubscriptList | MultiTypeSubscriptList
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

