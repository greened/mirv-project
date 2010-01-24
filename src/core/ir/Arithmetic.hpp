#ifndef mirv_core_ir_Arithmetic_hpp
#define mirv_core_ir_Arithmetic_hpp

#include <mirv/core/ir/Expression.hpp>
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
     typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface to a subtract expression.
  class Subtract {
  public:
    typedef boost::mpl::vector<Arithmetic, Associative,
			       Transitive, 
			       Reflexive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface to a multiply expression.
  class Multiply {
  public:
    typedef boost::mpl::vector<Arithmetic, Commutative, 
			       Associative, Transitive, 
			       Reflexive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface to a divide expression.
  class Divide {
  public:
    typedef boost::mpl::vector<Arithmetic,
			       Associative, Transitive, 
			       Reflexive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface to a modulo expression.
  class Modulus {
  public:
    typedef boost::mpl::vector<Arithmetic, 
			       Transitive, 
			       Reflexive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface to a negate expression.
  class Negate {
  public:
    typedef boost::mpl::vector<Arithmetic, 
			       Reflexive> Properties;
    typedef Expression<Unary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Unary> >::type BaseType;
  };
};

#endif
