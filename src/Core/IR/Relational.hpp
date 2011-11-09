#ifndef mirv_Core_IR_Relational_hpp
#define mirv_Core_IR_Relational_hpp

#include <mirv/Core/IR/RelationalFwd.hpp>
#include <mirv/Core/IR/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the interface for less-than expressions.
  class LessThan {
  public:
    typedef Expression<Transitive> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      LessThan,
      Relational,
      Transitive>::type BaseType;
  };

  /// Specify the interface for less-than-or-equal expressions.
  class LessThanOrEqual { 
  public:
    typedef Expression<Transitive> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      LessThanOrEqual,
      Relational,
      Transitive>::type BaseType;
  };

  /// Specify the interface for equal expressions.
  class Equal {
  public:
    typedef Expression<Transitive> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      Equal,
      Relational,
      Transitive>::type BaseType;
  };

  /// Specify the interface for not-equal expressions.
  class NotEqual {
  public:
    typedef Expression<Transitive> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      NotEqual,
      Relational,
      Transitive>::type BaseType;
  };

  /// Specify the interface for greater-than-or-equal expressions.
  class GreaterThanOrEqual {
  public:
    typedef Expression<Transitive> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      GreaterThanOrEqual,
      Relational,
      Transitive>::type BaseType;
  };

  /// Specify the interface for greater-than expressions.
  class GreaterThan { 
  public:
    typedef Expression<Transitive> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      GreaterThan,
      Relational,
      Transitive>::type BaseType;
  };
}

#endif
