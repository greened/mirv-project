#ifndef mirv_core_ir_Relational_hpp
#define mirv_core_ir_Relational_hpp

#include <mirv/core/ir/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the interface for less-than expressions.
  class LessThan {
  public:
    typedef boost::mpl::vector<Relational, Transitive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface for less-than-or-equal expressions.
  class LessThanOrEqual { 
  public:
    typedef boost::mpl::vector<Relational, Transitive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface for equal expressions.
  class Equal {
  public:
    typedef boost::mpl::vector<Relational, Transitive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface for not-equal expressions.
  class NotEqual {
  public:
    typedef boost::mpl::vector<Relational, Transitive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface for greater-than-or-equal expressions.
  class GreaterThanOrEqual {
  public:
    typedef boost::mpl::vector<Relational, Transitive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };

  /// Specify the interface for greater-than expressions.
  class GreaterThan { 
  public:
    typedef boost::mpl::vector<Relational, Transitive> Properties;
    typedef Expression<Binary> VisitorBaseType;
    typedef ExpressionBaseGenerator<Properties, Expression<Binary> >::type BaseType;
  };
}

#endif
