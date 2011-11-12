#ifndef mirv_Core_IR_Relational_hpp
#define mirv_Core_IR_Relational_hpp

#include <mirv/Core/IR/RelationalFwd.hpp>
#include <mirv/Core/IR/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the interface for less-than expressions.
  class LessThan {};

  /// Specify the interface for less-than-or-equal expressions.
  class LessThanOrEqual {};

  /// Specify the interface for equal expressions.
  class Equal {};

  /// Specify the interface for not-equal expressions.
  class NotEqual {};

  /// Specify the interface for greater-than-or-equal expressions.
  class GreaterThanOrEqual {};

  /// Specify the interface for greater-than expressions.
  class GreaterThan {};
}

#endif
