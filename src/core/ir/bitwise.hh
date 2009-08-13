#ifndef MIRVBitwise_hh
#define MIRVBitwise_hh

#include <mirv/expression.hh>
#include <boost/mpl/vector.hpp>

namespace MIRV {
   
   class BitwiseAnd {
   private:
      typedef boost::mpl::vector<BitwiseSemantic, CommutativeSemantic,
         AssociativeSemantic, TransitiveSemantic,
         ReflexiveSemantic, BitAndSemantic> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };
   
   class BitwiseOr {
   private:
      typedef boost::mpl::vector<BitwiseSemantic, CommutativeSemantic,
         AssociativeSemantic, TransitiveSemantic,
         ReflexiveSemantic, BitOrSemantic> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };
   
   class Complement {
   private:
      typedef boost::mpl::vector<BitwiseSemantic> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
   };
   

   class ShiftLeft {
   private:
      typedef boost::mpl::vector<BitwiseSemantic> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };

   class ArithmeticShiftRight {
   private:
      typedef boost::mpl::vector<BitwiseSemantic> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };

   class LogicalShiftRight {
   private:
      typedef boost::mpl::vector<BitwiseSemantic> sequence;
      
   public:
      typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
   };
};

#endif
