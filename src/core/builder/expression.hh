#ifndef mirv_core_builder_expression_hh
#define mirv_core_builder_expression_hh

#include <mirv/core/builder/transform.hh>
#include <mirv/core/ir/variable.hh>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     typedef boost::proto::terminal<ptr<Symbol<Variable> >::type>::type Variable;

     struct ConstructExpressionGrammar;

     // Unary expressions

     // No Posit
     typedef boost::proto::negate<ConstructExpressionGrammar> Negate;
     typedef boost::proto::complement<ConstructExpressionGrammar> Complement;
     typedef boost::proto::dereference<ConstructExpressionGrammar> Dereference;
     typedef boost::proto::address_of<ConstructExpressionGrammar> AddressOf;
     typedef boost::proto::logical_not<ConstructExpressionGrammar> Not;

      // No increment or decrement

      // Binary expressions
     typedef boost::proto::plus<ConstructExpressionGrammar, ConstructExpressionGrammar> Add;
     typedef boost::proto::minus<ConstructExpressionGrammar, ConstructExpressionGrammar> Minus;
     typedef boost::proto::multiplies<ConstructExpressionGrammar, ConstructExpressionGrammar> Multiplies;
     typedef boost::proto::divides<ConstructExpressionGrammar, ConstructExpressionGrammar> Divides;
     typedef boost::proto::shift_left<ConstructExpressionGrammar, ConstructExpressionGrammar> ShiftLeft;
     typedef boost::proto::shift_right<ConstructExpressionGrammar, ConstructExpressionGrammar> ShiftRight;
     typedef boost::proto::modulus<ConstructExpressionGrammar, ConstructExpressionGrammar> Modulus;
     typedef boost::proto::greater<ConstructExpressionGrammar, ConstructExpressionGrammar> Greater;
     typedef boost::proto::less<ConstructExpressionGrammar, ConstructExpressionGrammar> Less;
     typedef boost::proto::greater_equal<ConstructExpressionGrammar, ConstructExpressionGrammar> GreaterEqual;
     typedef boost::proto::less_equal<ConstructExpressionGrammar, ConstructExpressionGrammar> LessEqual;
     typedef boost::proto::equal_to<ConstructExpressionGrammar, ConstructExpressionGrammar> Equal;
     typedef boost::proto::not_equal_to<ConstructExpressionGrammar, ConstructExpressionGrammar> NotEqual;
     typedef boost::proto::logical_or<ConstructExpressionGrammar, ConstructExpressionGrammar> Or;
     typedef boost::proto::logical_and<ConstructExpressionGrammar, ConstructExpressionGrammar> And;
     typedef boost::proto::bitwise_or<ConstructExpressionGrammar, ConstructExpressionGrammar> BitwiseOr;
     typedef boost::proto::bitwise_and<ConstructExpressionGrammar, ConstructExpressionGrammar> BitwiseAnd;
     typedef boost::proto::bitwise_xor<ConstructExpressionGrammar, ConstructExpressionGrammar> BitwiseXor;

      // No comma

     typedef boost::proto::mem_ptr<ConstructExpressionGrammar, ConstructExpressionGrammar> MemPtr;

      // No assign (statement)
      // No op-assign

     typedef boost::proto::subscript<ConstructExpressionGrammar, ConstructExpressionGrammar> Subscript;

      // No ?:
      // Nary expressions

     typedef boost::proto::function<ConstructExpressionGrammar, 
				    boost::proto::vararg<ConstructExpressionGrammar> > Call;
   }
}

#endif
