#ifndef mirv_Core_Builder_ExpressionRules_hpp
#define mirv_Core_Builder_ExpressionRules_hpp

#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/GrammarFwd.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Function.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     // Unary expressions

     // No Posit
     typedef boost::proto::negate<ConstructExpressionGrammar> NegateRule;
     typedef boost::proto::complement<ConstructExpressionGrammar> ComplementRule;
     typedef boost::proto::dereference<ConstructExpressionGrammar> DereferenceRule;
     typedef boost::proto::address_of<ConstructExpressionGrammar> AddressOfRule;
     typedef boost::proto::logical_not<ConstructExpressionGrammar> NotRule;

      // No increment or decrement

      // Binary expressions
     typedef boost::proto::plus<ConstructExpressionGrammar, ConstructExpressionGrammar> AddRule;
     typedef boost::proto::minus<ConstructExpressionGrammar, ConstructExpressionGrammar> MinusRule;
     typedef boost::proto::multiplies<ConstructExpressionGrammar, ConstructExpressionGrammar> MultipliesRule;
     typedef boost::proto::divides<ConstructExpressionGrammar, ConstructExpressionGrammar> DividesRule;
     typedef boost::proto::shift_left<ConstructExpressionGrammar, ConstructExpressionGrammar> ShiftLeftRule;
     typedef boost::proto::shift_right<ConstructExpressionGrammar, ConstructExpressionGrammar> ShiftRightRule;
     typedef boost::proto::modulus<ConstructExpressionGrammar, ConstructExpressionGrammar> ModulusRule;
     typedef boost::proto::greater<ConstructExpressionGrammar, ConstructExpressionGrammar> GreaterRule;
     typedef boost::proto::less<ConstructExpressionGrammar, ConstructExpressionGrammar> LessRule;
     typedef boost::proto::greater_equal<ConstructExpressionGrammar, ConstructExpressionGrammar> GreaterEqualRule;
     typedef boost::proto::less_equal<ConstructExpressionGrammar, ConstructExpressionGrammar> LessEqualRule;
     typedef boost::proto::equal_to<ConstructExpressionGrammar, ConstructExpressionGrammar> EqualRule;
     typedef boost::proto::not_equal_to<ConstructExpressionGrammar, ConstructExpressionGrammar> NotEqualRule;
     typedef boost::proto::logical_or<ConstructExpressionGrammar, ConstructExpressionGrammar> OrRule;
     typedef boost::proto::logical_and<ConstructExpressionGrammar, ConstructExpressionGrammar> AndRule;
     typedef boost::proto::bitwise_or<ConstructExpressionGrammar, ConstructExpressionGrammar> BitwiseOrRule;
     typedef boost::proto::bitwise_and<ConstructExpressionGrammar, ConstructExpressionGrammar> BitwiseAndRule;
     typedef boost::proto::bitwise_xor<ConstructExpressionGrammar, ConstructExpressionGrammar> BitwiseXorRule;

     typedef boost::proto::mem_ptr<ConstructExpressionGrammar, ConstructExpressionGrammar> MemPtrRule;

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
       ConstructExpressionGrammar,
       MultiIndexRule
       > {};

     /// Match a subscript expression with multiple indices.
     /// MultiSubscript -> Expression[MultiIndex]
     struct MultiSubscriptRule :  boost::proto::subscript<
       ConstructExpressionGrammar,
       MultiIndexRule
       > {};

     /// This is the rule to match single subscript expressions.
     /// Subscript -> Expression[Expression]
     struct SubscriptRule : boost::proto::subscript<
       ConstructExpressionGrammar,
       ConstructExpressionGrammar
       > {};

     /// Match an expression returning an address of a subscript
     /// expression with multiple subscripts.
     /// &expr[expr(,expr)*]
     struct MultiSubscriptAddressRule : boost::proto::address_of<
       boost::proto::subscript<
         ConstructExpressionGrammar,
         MultiIndexRule
         >
       > {};

     struct SubscriptAddressRule : boost::proto::address_of<
       boost::proto::subscript<
         ConstructExpressionGrammar,
         ConstructExpressionGrammar
         >
       > {};

     // No comma
     // No assign (statement)
     // No op-assign
     // No ?:
   }
}

#endif
