#ifndef mirv_core_builder_expression_hh
#define mirv_core_builder_expression_hh

#include <mirv/core/builder/transform.hh>
#include <mirv/core/ir/variable.hh>

namespace mirv {
   namespace Builder {
     typedef boost::proto::terminal<Symbol<Variable> >::type Variable;

     template<typename T>
     struct Type
       : boost::proto::terminal< convertible_to<T> >
     {};

     struct ExpressionGrammar;

     // Unary expressions

     // No Posit
      struct Negate
            : boost::proto::negate<ExpressionGrammar> {};

      struct Complement
            : boost::proto::complement<ExpressionGrammar> {};

      struct Dereference
            : boost::proto::dereference<ExpressionGrammar> {};

      struct AddressOf
            : boost::proto::address_of<ExpressionGrammar> {};

      struct Not
            : boost::proto::logical_not<ExpressionGrammar> {};

      // No increment or decrement

      // Binary expressions
      struct Add
            : boost::proto::plus<ExpressionGrammar, ExpressionGrammar> {};

      struct Minus
            : boost::proto::minus<ExpressionGrammar, ExpressionGrammar> {};

      struct Multiplies
            : boost::proto::multiplies<ExpressionGrammar, ExpressionGrammar> {};

      struct Divides
            : boost::proto::divides<ExpressionGrammar, ExpressionGrammar> {};

      struct ShiftLeft
            : boost::proto::shift_left<ExpressionGrammar, ExpressionGrammar> {};

      struct ShiftRight
            : boost::proto::shift_right<ExpressionGrammar, ExpressionGrammar> {};

      struct Modulus
            : boost::proto::modulus<ExpressionGrammar, ExpressionGrammar> {};

      struct Greater
            : boost::proto::greater<ExpressionGrammar, ExpressionGrammar> {};

      struct Less
            : boost::proto::less<ExpressionGrammar, ExpressionGrammar> {};

      struct GreaterEqual
            : boost::proto::greater_equal<ExpressionGrammar, ExpressionGrammar> {};

      struct LessEqual
            : boost::proto::less_equal<ExpressionGrammar, ExpressionGrammar> {};

      struct Equal
            : boost::proto::equal_to<ExpressionGrammar, ExpressionGrammar> {};

      struct NotEqual
            : boost::proto::not_equal_to<ExpressionGrammar, ExpressionGrammar> {};

      struct Or
            : boost::proto::logical_or<ExpressionGrammar, ExpressionGrammar> {};

      struct And
            : boost::proto::logical_and<ExpressionGrammar, ExpressionGrammar> {};

      struct BitwiseOr
            : boost::proto::bitwise_or<ExpressionGrammar, ExpressionGrammar> {};

      struct BitwiseAnd
            : boost::proto::bitwise_and<ExpressionGrammar, ExpressionGrammar> {};

      struct BitwiseXor
            : boost::proto::bitwise_xor<ExpressionGrammar, ExpressionGrammar> {};

      // No comma

      struct MemPtr
            : boost::proto::mem_ptr<ExpressionGrammar, ExpressionGrammar> {};

      // No assign (statement)
      // No op-assign

      struct Subscript
            : boost::proto::subscript<ExpressionGrammar, ExpressionGrammar> {};

      // No ?:
      // Nary expressions

      struct Call
            : boost::proto::function<ExpressionGrammar, 
                                     boost::proto::vararg<ExpressionGrammar> > {};
   }
}

#endif
