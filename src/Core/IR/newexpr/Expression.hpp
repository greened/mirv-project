#ifndef mirv_Core_IR_Expression_hpp
#define mirv_Core_IR_Expression_hpp

#include <Core/Containers/Vector.hpp>
#include <Core/IR/Index.hpp>
#include <Core/IR/Node.hpp>

#include <boost/range/iterator_range.hpp>

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
  };
}

#endif
