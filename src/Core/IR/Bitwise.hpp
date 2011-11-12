#ifndef mirv_Core_IR_Bitwise_hpp
#define mirv_Core_IR_Bitwise_hpp

#include <mirv/Core/IR/BitwiseFwd.hpp>
#include <mirv/Core/IR/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the bitwise and interface.
   class BitwiseAnd {};
   
  /// Specify the bitwise or interface.
   class BitwiseOr {};

  /// Specify the bitwise xor interface.
   class BitwiseXor {};
   
  /// Specify the bitwise complement interface.
   class BitwiseComplement {};
   

  /// Specify the bitwise left shift interface.
   class ShiftLeft {};

  /// Specify the bitwise arithmetic right shift interface.
   class ArithmeticShiftRight {};

  /// Specify the bitwise logical right shift interface.
   class LogicalShiftRight {};
};

#endif
