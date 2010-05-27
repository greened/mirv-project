#ifndef mirv_Core_IR_Bitwise_hpp
#define mirv_Core_IR_Bitwise_hpp

#include <mirv/Core/IR/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the bitwise and interface.
   class BitwiseAnd {
   public:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> Properties;
     typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Binary>,
       BitwiseAnd>::type BaseType;
   };
   
  /// Specify the bitwise or interface.
   class BitwiseOr {
   public:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> Properties;
     typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Binary>,
       BitwiseOr>::type BaseType;
   };

  /// Specify the bitwise xor interface.
   class BitwiseXor {
   public:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> Properties;
     typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Binary>,
       BitwiseXor>::type BaseType;
   };
   
  /// Specify the bitwise complement interface.
   class BitwiseComplement {
   public:
      typedef boost::mpl::vector<Bitwise> Properties;
     typedef Expression<Unary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Unary>,
       BitwiseComplement>::type BaseType;
   };
   

  /// Specify the bitwise left shift interface.
   class ShiftLeft {
   public:
      typedef boost::mpl::vector<Bitwise> Properties;
    typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Binary>,
       ShiftLeft>::type BaseType;
   };

  /// Specify the bitwise arithmetic right shift interface.
   class ArithmeticShiftRight {
   public:
      typedef boost::mpl::vector<Bitwise> Properties;
    typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Binary>,
       ArithmeticShiftRight>::type BaseType;
   };

  /// Specify the bitwise logical right shift interface.
   class LogicalShiftRight {
   public:
      typedef boost::mpl::vector<Bitwise> Properties;
    typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Binary>,
       LogicalShiftRight>::type BaseType;
   };
};

#endif
