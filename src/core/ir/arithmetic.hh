#ifndef MIRVArithmetic_hh
#define MIRVArithmetic_hh

#include <mirv/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace MIRV {

  // Expression Ops
  class Add {
  private:
    typedef boost::mpl::vector<ArithmeticSemantic, CommutativeSemantic, 
			       AssociativeSemantic, TransitiveSemantic, 
			       ReflexiveSemantic, AddSemantic> sequence;

  public:
     typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };


  class Subtract {
  private:
    typedef boost::mpl::vector<ArithmeticSemantic, AssociativeSemantic, 
			       TransitiveSemantic, 
			       ReflexiveSemantic, SubSemantic> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };


  class Multiply {
  private:
    typedef boost::mpl::vector<ArithmeticSemantic, CommutativeSemantic, 
			       AssociativeSemantic, TransitiveSemantic, 
			       ReflexiveSemantic, MultSemantic> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class Divide {
  private:
    typedef boost::mpl::vector<ArithmeticSemantic,
			       AssociativeSemantic, TransitiveSemantic, 
			       ReflexiveSemantic, DivSemantic> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class Modulus {
  private:
    typedef boost::mpl::vector<ArithmeticSemantic, 
			       TransitiveSemantic, 
			       ReflexiveSemantic, ModSemantic> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class Negate {
  private:
    typedef boost::mpl::vector<ArithmeticSemantic, 
			       ReflexiveSemantic, NegSemantic> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
  };
};

#endif
