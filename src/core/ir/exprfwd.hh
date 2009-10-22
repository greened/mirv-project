#ifndef mirv_core_ir_exprfwd_hh
#define mirv_core_ir_exprhwd_hh

namespace mirv {
   // Tags
   class Unary;
   class Binary;
   class Commutative;
   class NonCommutative;
   class Associative;
   class NonAssociative;
   class Transitive;
   class Intransitive;
   class Reflexive;
   class NonReflexive;

   // Arithmetic
   class Add;
   class Subtract;
   class Multiply;
   class Divide;
   class Modulus;
   class Negate;

   // Logical
   class LogicalAnd;
   class LogicalOr;
   class Not;

   // Bitwise
   class BitwiseAnd;
   class BitwiseOr;
   class BitwiseXor;
   class Complement;
   class ShiftLeft;
   class ArithmeticShiftRight;
   class LogicalShiftRight;

   // Relational
   class LessThan;
   class LessThanOrEqual;
   class Equal;
   class NotEqual;
   class GreaterThanOrEqual;
   class GreaterThan;

   // Expression template
   template<typename Op, typename Base>
   class Expression;
}

#endif
