#ifndef MIRVBitwise_hh
#define MIRVBitwise_hh

#include <mirv/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace MIRV {
   
   class BitwiseAnd {
   private:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };
   
   class BitwiseOr {
   private:
      typedef boost::mpl::vector<Bitwise, Commutative,
         Associative, Transitive,
         Reflexive> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };
   
   class BitwiseComplement {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
   };
   

   class ShiftLeft {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };

   class ArithmeticShiftRight {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };

   class LogicalShiftRight {
   private:
      typedef boost::mpl::vector<Bitwise> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };
};

#endif
