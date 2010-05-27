#ifndef mirv_Core_IR_Arithmetic_hpp
#define mirv_Core_IR_Arithmetic_hpp

#include <mirv/Core/IR/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  // Expression Ops

  /// Specify the interface to an add expression.
  class Add {
  public:
    typedef boost::mpl::vector<Arithmetic, Commutative, 
			       Associative, Transitive, 
			       Reflexive> Properties;
     typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary>,
      Add>::type BaseType;
  };

  /// Specify the interface to a subtract expression.
  class Subtract {
  public:
    typedef boost::mpl::vector<Arithmetic, Associative,
			       Transitive, 
			       Reflexive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary>,
      Subtract>::type BaseType;
  };

  /// Specify the interface to a multiply expression.
  class Multiply {
  public:
    typedef boost::mpl::vector<Arithmetic, Commutative, 
			       Associative, Transitive, 
			       Reflexive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary>,
      Multiply>::type BaseType;
  };

  /// Specify the interface to a divide expression.
  class Divide {
  public:
    typedef boost::mpl::vector<Arithmetic,
			       Associative, Transitive, 
			       Reflexive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary>,
      Divide>::type BaseType;
  };

  /// Specify the interface to a modulo expression.
  class Modulus {
  public:
    typedef boost::mpl::vector<Arithmetic, 
			       Transitive, 
			       Reflexive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary>,
      Modulus>::type BaseType;
  };

  /// Specify the interface to a negate expression.
  class Negate {
  public:
    typedef boost::mpl::vector<Arithmetic, 
			       Reflexive> Properties;
    typedef Expression<Unary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Unary>,
      Negate>::type BaseType;
  };
};

#endif
