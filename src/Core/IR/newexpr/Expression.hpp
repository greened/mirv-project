#ifndef mirv_Core_IR_Expression_hpp
#define mirv_Core_IR_Expression_hpp

#include <Core/IR/Node.hpp>
#include <Core/IR/Type.hpp>

namespace mirv {
  class Expression : public Node<Expression> {
  public:
    enum Kind {
      Null,

      // Arithmetic
      Add,
      Subtract,
      Multiply,
      Divide,
      Modulus,
      Negate,

      // Logical
      LogicalAnd,
      LogicalOr,
      LogicalNot,

      // Bitwise
      BitwiseAnd,
      BitwiseOr,
      BitwiseComplement,
      BitwiseXor,
      ShiftLeft,
      ArithmeticShiftRight,
      LogicalShiftRight,

      // Relational
      LessThan,
      LessThanOrEqual,
      Equal,
      NotEqual,
      GreaterThanOrEqual,
      GreaterThan,

      // Reference
      Reference,
      Load,
      TuplePointer
    };

    typedef KindTuple<Kind,
                      Add, Subtract, Multiply, Divide, Modulus, Negate,
                      LogicalAnd, LogicalOr,
                      BitwiseAnd, BitwiseOr, BitwiseXor, ShiftLeft,
                      ArithmeticShiftRight, LogicalShiftRight,
                      LessThan, LessThanOrEqual, Equal, NotEqual,
                      GreaterThanOrEqual, GreaterThan,
                      Reference, Load, TuplePointer> Kinds;

    Kind kind(void) const {
      return theKind;
    }

  private:
    Kind theKind;
    Index<Type> theType;
  };
}

#endif
