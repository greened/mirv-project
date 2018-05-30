#ifndef mirv_Core_Builder_ExpressionRules_hpp
#define mirv_Core_Builder_ExpressionRules_hpp

#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/GrammarFwd.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/IR/Function.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     class ExpressionRule;

     /// Define the rules for the expression builder grammar.
     struct ExpressionRuleCases {
       /// This is the default case.  It matches nothing so that
       /// illegal constructs will flag an error.
       template<typename Tag>
       struct case_ : boost::proto::not_<boost::proto::_> {};
     };

     // Unary expressions

     // No Posit
     typedef boost::proto::negate<ExpressionRule> NegateRule;
     typedef boost::proto::complement<ExpressionRule> ComplementRule;
     typedef boost::proto::dereference<ExpressionRule> DereferenceRule;
     typedef boost::proto::address_of<ExpressionRule> AddressOfRule;
     typedef boost::proto::logical_not<ExpressionRule> NotRule;

      // No increment or decrement

      // Binary expressions
     typedef boost::proto::plus<ExpressionRule, ExpressionRule> AddRule;
     typedef boost::proto::minus<ExpressionRule, ExpressionRule> MinusRule;
     typedef boost::proto::multiplies<ExpressionRule, ExpressionRule> MultipliesRule;
     typedef boost::proto::divides<ExpressionRule, ExpressionRule> DividesRule;
     typedef boost::proto::shift_left<ExpressionRule, ExpressionRule> ShiftLeftRule;
     typedef boost::proto::shift_right<ExpressionRule, ExpressionRule> ShiftRightRule;
     typedef boost::proto::modulus<ExpressionRule, ExpressionRule> ModulusRule;
     typedef boost::proto::greater<ExpressionRule, ExpressionRule> GreaterRule;
     typedef boost::proto::less<ExpressionRule, ExpressionRule> LessRule;
     typedef boost::proto::greater_equal<ExpressionRule, ExpressionRule> GreaterEqualRule;
     typedef boost::proto::less_equal<ExpressionRule, ExpressionRule> LessEqualRule;
     typedef boost::proto::equal_to<ExpressionRule, ExpressionRule> EqualRule;
     typedef boost::proto::not_equal_to<ExpressionRule, ExpressionRule> NotEqualRule;
     typedef boost::proto::logical_or<ExpressionRule, ExpressionRule> OrRule;
     typedef boost::proto::logical_and<ExpressionRule, ExpressionRule> AndRule;
     typedef boost::proto::bitwise_or<ExpressionRule, ExpressionRule> BitwiseOrRule;
     typedef boost::proto::bitwise_and<ExpressionRule, ExpressionRule> BitwiseAndRule;
     typedef boost::proto::bitwise_xor<ExpressionRule, ExpressionRule> BitwiseXorRule;

     typedef boost::proto::mem_ptr<ExpressionRule, ExpressionRule> MemPtrRule;

     // Array reference rules.
     struct ArrayIndexRule;

     /// This is the rule to match a list of indices.
     /// MultiIndex -> ArrayIndex, ArrayIndex
     struct MultiIndexRule
         : public boost::proto::comma<
       ArrayIndexRule,
       ArrayIndexRule
       > {};

     /// This is the rule to match an array subscript.
     /// ArrayIndex -> Expression | MultiIndex
     struct ArrayIndexRule : public boost::proto::or_<
       ExpressionRule,
       MultiIndexRule
       > {};

     /// Match a subscript expression with multiple indices.
     /// MultiSubscript -> Expression[MultiIndex]
     struct MultiSubscriptRule :  boost::proto::subscript<
       ExpressionRule,
       MultiIndexRule
       > {};

     /// This is the rule to match single subscript expressions.
     /// Subscript -> Expression[Expression]
     struct SubscriptRule : boost::proto::subscript<
       ExpressionRule,
       ExpressionRule
       > {};

     /// Match an expression returning an address of a subscript
     /// expression with multiple subscripts.
     /// &expr[expr(,expr)*]
     struct MultiSubscriptAddressRule : boost::proto::address_of<
       boost::proto::subscript<
         ExpressionRule,
         MultiIndexRule
         >
       > {};

     struct SubscriptAddressRule : boost::proto::address_of<
       boost::proto::subscript<
         ExpressionRule,
         ExpressionRule
         >
       > {};

     // No comma
     // No assign (statement)
     // No op-assign
     // No ?:

     struct ExpressionRule : boost::proto::or_<
       >;
   }
}

#endif
