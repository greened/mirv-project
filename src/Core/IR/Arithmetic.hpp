#ifndef mirv_Core_IR_Arithmetic_hpp
#define mirv_Core_IR_Arithmetic_hpp

#include <mirv/Core/IR/ArithmeticFwd.hpp>
#include <mirv/Core/IR/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  // Expression Ops

  /// Specify the interface to an add expression.
  class Add {};

  /// Specify the interface to a subtract expression.
  class Subtract {};

  /// Specify the interface to a multiply expression.
  class Multiply {};

  /// Specify the interface to a divide expression.
  class Divide {};

  /// Specify the interface to a modulo expression.
  class Modulus {};

  /// Specify the interface to a negate expression.
  class Negate {};
};

#endif
