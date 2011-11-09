#ifndef mirv_Core_IR_Arithmetic_hpp
#define mirv_Core_IR_Arithmetic_hpp

#include <mirv/Core/IR/ArithmeticFwd.hpp>
#include <mirv/Core/IR/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  // Expression Ops

  /// Specify the interface to an add expression.
  class Add {
  public:
    typedef Expression<Commutative> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      Add,
      Arithmetic,
      Associative,
      Commutative,
      Reflexive,
      Transitive>::type BaseType;
  };

  /// Specify the interface to a subtract expression.
  class Subtract {
  public:
    typedef Expression<Associative> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      Subtract,
      Arithmetic,
      Associative,
      Reflexive,
      Transitive>::type BaseType;
  };

  /// Specify the interface to a multiply expression.
  class Multiply {
  public:
    typedef Expression<Commutative> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      Multiply,
      Arithmetic,
      Associative,
      Commutative,
      Reflexive,
      Transitive
      >::type BaseType;
  };

  /// Specify the interface to a divide expression.
  class Divide {
  public:
    typedef Expression<Transitive> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      Divide,
      Arithmetic,
      Reflexive,
      Transitive>::type BaseType;
  };

  /// Specify the interface to a modulo expression.
  class Modulus {
  public:
    typedef Expression<Transitive> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      Modulus,
      Arithmetic,
      Reflexive,
      Transitive>::type BaseType;
  };

  /// Specify the interface to a negate expression.
  class Negate {
  public:
    typedef Expression<Reflexive> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Unary>,
      Negate,
      Arithmetic,
      Reflexive>::type BaseType;
  };
};

#endif
