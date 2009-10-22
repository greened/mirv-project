#ifndef mirv_core_ir_arithmetic_hh
#define mirv_core_ir_arithmetic_hh

#include <mirv/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
  // Expression Ops
  class Add {
  private:
    typedef boost::mpl::vector<Arithmetic, Commutative, 
			       Associative, Transitive, 
			       Reflexive> sequence;

  public:
     typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };


  class Subtract {
  private:
    typedef boost::mpl::vector<Arithmetic, Associative, 
			       Transitive, 
			       Reflexive> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };


  class Multiply {
  private:
    typedef boost::mpl::vector<Arithmetic, Commutative, 
			       Associative, Transitive, 
			       Reflexive> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class Divide {
  private:
    typedef boost::mpl::vector<Arithmetic,
			       Associative, Transitive, 
			       Reflexive> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class Modulus {
  private:
    typedef boost::mpl::vector<Arithmetic, 
			       Transitive, 
			       Reflexive> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, BinaryExpression>::type base_type;
  };

  class Negate {
  private:
    typedef boost::mpl::vector<Arithmetic, 
			       Reflexive> sequence;

  public:
    typedef ExpressionBaseGenerator<sequence, UnaryExpression>::type base_type;
  };
};

#endif
