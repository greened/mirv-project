#ifndef mirv_Core_IR_Bitwise_hpp
#define mirv_Core_IR_Bitwise_hpp

#include <mirv/Core/IR/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the bitwise and interface.
   class BitwiseAnd {
   public:
     typedef Expression<Commutative> VisitorBaseType;
     typedef ExpressionBaseGenerator<
       Expression<Binary>,
       BitwiseAnd,
       Associative,
       Bitwise,
       Commutative,
       Reflexive,
       Transitive>::type BaseType;
   };
   
  /// Specify the bitwise or interface.
   class BitwiseOr {
   public:
     typedef Expression<Commutative> VisitorBaseType;
     typedef ExpressionBaseGenerator<
       Expression<Binary>,
       BitwiseOr,
       Associative,
       Bitwise,
       Commutative,
       Reflexive,
       Transitive>::type BaseType;
   };

  /// Specify the bitwise xor interface.
   class BitwiseXor {
   public:
     typedef Expression<Commutative> VisitorBaseType;
     typedef ExpressionBaseGenerator<
       Expression<Binary>,
       BitwiseXor,
       Associative,
       Bitwise,
       Commutative,
       Reflexive,
       Transitive>::type BaseType;
   };
   
  /// Specify the bitwise complement interface.
   class BitwiseComplement {
   public:
     typedef Expression<Bitwise> VisitorBaseType;
     typedef ExpressionBaseGenerator<
       Expression<Unary>,
       BitwiseComplement,
       Bitwise>::type BaseType;
   };
   

  /// Specify the bitwise left shift interface.
   class ShiftLeft {
   public:
    typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<
       Expression<Binary>,
       ShiftLeft,
       Bitwise>::type BaseType;
   };

  /// Specify the bitwise arithmetic right shift interface.
   class ArithmeticShiftRight {
   public:
    typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<
       Expression<Binary>,
       ArithmeticShiftRight,
       Bitwise>::type BaseType;
   };

  /// Specify the bitwise logical right shift interface.
   class LogicalShiftRight {
   public:
    typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<
       Expression<Binary>,
       LogicalShiftRight,
       Bitwise>::type BaseType;
   };
};

#endif
