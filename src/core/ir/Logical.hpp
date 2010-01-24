#ifndef mirv_core_ir_Logical_hpp
#define mirv_core_ir_Logical_hpp

#include <mirv/core/ir/Expression.hpp>
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
