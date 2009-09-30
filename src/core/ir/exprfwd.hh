#ifndef MIRV_Core_IR_Exprfwd_hh
#define MIRV_Core_IR_Exprhwd_hh

namespace MIRV {
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
