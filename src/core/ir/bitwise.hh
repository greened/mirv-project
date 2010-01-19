#ifndef mirv_core_ir_bitwise_hh
#define mirv_core_ir_bitwise_hh

#include <mirv/core/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the bitwise and interface.
   class BitwiseAnd {
   private:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> sequence;
      
   public:
    typedef sequence properties;
     typedef Expression<Binary> visitor_base_type;
     typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
   };
   
  /// Specify the bitwise or interface.
   class BitwiseOr {
   private:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> sequence;
      
   public:
    typedef sequence properties;
     typedef Expression<Binary> visitor_base_type;
     typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
   };

  /// Specify the bitwise xor interface.
   class BitwiseXor {
   private:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> sequence;
      
   public:
    typedef sequence properties;
     typedef Expression<Binary> visitor_base_type;
     typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
   };
   
  /// Specify the bitwise complement interface.
   class BitwiseComplement {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
    typedef sequence properties;
     typedef Expression<Unary> visitor_base_type;
     typedef ExpressionBaseGenerator<sequence, Expression<Unary> >::type base_type;
   };
   

  /// Specify the bitwise left shift interface.
   class ShiftLeft {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
     typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
     typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
   };

  /// Specify the bitwise arithmetic right shift interface.
   class ArithmeticShiftRight {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
      typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
   };

  /// Specify the bitwise logical right shift interface.
   class LogicalShiftRight {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
      typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
   };
};

#endif
