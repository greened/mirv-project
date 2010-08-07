#ifndef mirv_Core_Builder_Expression_hpp
#define mirv_Core_Builder_Expression_hpp

#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/DomainFwd.hpp>
#include <mirv/Core/Builder/GrammarFwd.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Function.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// Define a convenient way to talk about variable reference
     /// expressions.
     typedef Wrapper<boost::proto::terminal<std::string>::type > VariableTerminal;

     /// Define a convenient way to talk about function reference
     /// expressions.
     typedef Wrapper<boost::proto::terminal<std::string>::type > FunctionTerminal;

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

      // No comma

     typedef boost::proto::mem_ptr<ConstructExpressionGrammar, ConstructExpressionGrammar> MemPtrRule;

      // No assign (statement)
      // No op-assign

     typedef boost::proto::subscript<ConstructExpressionGrammar, ConstructExpressionGrammar> SubscriptRule;

      // No ?:
      // Nary expressions

     typedef boost::proto::function<ConstructExpressionGrammar, 
				    boost::proto::vararg<ConstructExpressionGrammar> > CallRule;
   }
}

#endif
