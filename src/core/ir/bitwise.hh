#ifndef mirv_core_ir_bitwise_hh
#define mirv_core_ir_bitwise_hh

#include <mirv/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
   class BitwiseAnd {
   private:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> sequence;
      
   public:
    typedef sequence properties;
    typedef BinaryExpression visitor_base_type;
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };
   
   class BitwiseOr {
   private:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> sequence;
      
   public:
    typedef sequence properties;
    typedef BinaryExpression visitor_base_type;
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };
   
   class BitwiseComplement {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
    typedef sequence properties;
    typedef UnaryExpression visitor_base_type;
      typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
   };
   

   class ShiftLeft {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
     typedef sequence properties;
    typedef BinaryExpression visitor_base_type;
     typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };

   class ArithmeticShiftRight {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
    typedef sequence properties;
    typedef BinaryExpression visitor_base_type;
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };

   class LogicalShiftRight {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
    typedef sequence properties;
    typedef BinaryExpression visitor_base_type;
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };
};

#endif
