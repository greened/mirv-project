#ifndef mirv_Core_IR_Logical_hpp
#define mirv_Core_IR_Logical_hpp

#include <mirv/Core/IR/LogicalFwd.hpp>
#include <mirv/Core/IR/Expression.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  /// Specify the interface for the logical and expression.
  class LogicalAnd { 
  public:
    typedef Expression<Commutative> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      LogicalAnd,
      Associative,
      Commutative,
      Logical,
      Reflexive,
      Transitive>::type BaseType;
  };

  /// Specify the interface for the logical or expression.
  class LogicalOr {
  public:
    typedef Expression<Commutative> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Binary>,
      LogicalOr,
      Associative,
      Commutative,
      Logical,
      Reflexive,
      Transitive>::type BaseType;
  };

  /// Specify the interface for the logical not expression.
  class LogicalNot {
  public:
    typedef Expression<Reflexive> VisitorBaseType;
    typedef ExpressionBaseGenerator<
      Expression<Unary>,
      LogicalNot,
      Logical,
      Reflexive>::type BaseType;
  };
};

#endif
