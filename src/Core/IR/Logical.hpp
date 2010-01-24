#ifndef mirv_Core_IR_Logical_hpp
#define mirv_Core_IR_Logical_hpp

#include <mirv/Core/IR/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the interface for the logical and expression.
  class LogicalAnd { 
  public:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface for the logical or expression.
  class LogicalOr {
  public:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface for the logical not expression.
  class LogicalNot {
  public:
    typedef boost::mpl::vector<Logical, Commutative,
			       Associative, Transitive,
			       Reflexive> Properties;
    typedef Expression<Unary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Unary> >::type BaseType;
  };

};

#endif
