#ifndef mirv_Core_IR_Expression_hpp
#define mirv_Core_IR_Expression_hpp

#include <Core/Containers/Vector.hpp>
#include <Core/IR/Index.hpp>

#include <boost/range/iterator_range.hpp>

namespace mirv {
  enum ExpressionKind {
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

  class Expression {
  private:
    ExpressionKind kind;

    typedef Vector<Index<Expression>> IndexVector;
    IndexVector children;

  public:
    typedef boost::iterator_range<IndexVector::iterator> range;
    typedef boost::iterator_range<VectorType::const_iterator> const_range;
  };
}

#endif
