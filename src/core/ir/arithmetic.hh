#ifndef mirv_core_ir_arithmetic_hh
#define mirv_core_ir_arithmetic_hh

#include <mirv/core/ir/expression.hh>
#include <boost/mpl/vector.hpp>

namespace mirv {
  // Expression Ops

  /// Specify the interface to an add expression.
  class Add {
  private:
    typedef boost::mpl::vector<Arithmetic, Commutative, 
			       Associative, Transitive, 
			       Reflexive> sequence;

  public:
    typedef sequence properties;
     typedef Expression<Binary> visitor_base_type;
     typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  /// Specify the interface to a subtract expression.
  class Subtract {
  private:
    typedef boost::mpl::vector<Arithmetic, Associative, 
			       Transitive, 
			       Reflexive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  /// Specify the interface to a multiply expression.
  class Multiply {
  private:
    typedef boost::mpl::vector<Arithmetic, Commutative, 
			       Associative, Transitive, 
			       Reflexive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  /// Specify the interface to a divide expression.
  class Divide {
  private:
    typedef boost::mpl::vector<Arithmetic,
			       Associative, Transitive, 
			       Reflexive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  /// Specify the interface to a modulo expression.
  class Modulus {
  private:
    typedef boost::mpl::vector<Arithmetic, 
			       Transitive, 
			       Reflexive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Binary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Binary> >::type base_type;
  };

  /// Specify the interface to a negate expression.
  class Negate {
  private:
    typedef boost::mpl::vector<Arithmetic, 
			       Reflexive> sequence;

  public:
    typedef sequence properties;
    typedef Expression<Unary> visitor_base_type;
    typedef ExpressionBaseGenerator<sequence, Expression<Unary> >::type base_type;
  };
};

#endif
