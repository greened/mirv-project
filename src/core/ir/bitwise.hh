#ifndef mirv_core_ir_bitwise_hh
#define mirv_core_ir_bitwise_hh

#include <mirv/core/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the bitwise and interface.
   class BitwiseAnd {
   public:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> Properties;
     typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
   };
   
  /// Specify the bitwise or interface.
   class BitwiseOr {
   public:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> Properties;
     typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
   };

  /// Specify the bitwise xor interface.
   class BitwiseXor {
   public:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> Properties;
     typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
   };
   
  /// Specify the bitwise complement interface.
   class BitwiseComplement {
   public:
      typedef boost::mpl::vector<Bitwise> Properties;
     typedef Expression<Unary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Unary> >::type BaseType;
   };
   

  /// Specify the bitwise left shift interface.
   class ShiftLeft {
   public:
      typedef boost::mpl::vector<Bitwise> Properties;
    typedef Expression<Binary> VisitorBaseType;
     typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
   };

  /// Specify the bitwise arithmetic right shift interface.
   class ArithmeticShiftRight {
   public:
      typedef boost::mpl::vector<Bitwise> Properties;
    typedef Expression<Binary> VisitorBaseType;
      typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
   };

  /// Specify the bitwise logical right shift interface.
   class LogicalShiftRight {
   public:
      typedef boost::mpl::vector<Bitwise> Properties;
    typedef Expression<Binary> VisitorBaseType;
      typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
   };
};

#endif
