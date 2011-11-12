#ifndef mirv_Core_IR_Logical_hpp
#define mirv_Core_IR_Logical_hpp

#include <mirv/Core/IR/LogicalFwd.hpp>
#include <mirv/Core/IR/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the interface for the logical and expression.
  class LogicalAnd {};

  /// Specify the interface for the logical or expression.
  class LogicalOr {};

  /// Specify the interface for the logical not expression.
  class LogicalNot {};
};

#endif
