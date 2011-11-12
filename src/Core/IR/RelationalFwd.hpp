#ifndef mirv_core_ir_relational_fwd_hh
#define mirv_core_ir_relational_fwd_hh

#include <mirv/Core/IR/ExpressionFwd.hpp>

namespace mirv {
  class LessThan;
  class LessThanOrEqual;
  class Equal;
  class NotEqual;
  class GreaterThanOrEqual;
  class GreaterThan;

  namespace detail {
    /// Define the visitation base type for less than expressions.
    template<>
    struct VisitorBaseTypeOfExpression<LessThan> {
      typedef Expression<Transitive> VisitorBaseType;
    };
    /// Define the base type of less than expressions.
    template<>
    struct BaseTypeOfExpression<LessThan> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        LessThan,
        Relational,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for less than or equal expressions.
    template<>
    struct VisitorBaseTypeOfExpression<LessThanOrEqual> {
      typedef Expression<Transitive> VisitorBaseType;
    };
    /// Define the base type of less than expressions.
    template<>
    struct BaseTypeOfExpression<LessThanOrEqual> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        LessThanOrEqual,
        Relational,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for equal expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Equal> {
      typedef Expression<Transitive> VisitorBaseType;
    };
    /// Define the base type of less than expressions.
    template<>
    struct BaseTypeOfExpression<Equal> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        Equal,
        Relational,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for not equal expressions.
    template<>
    struct VisitorBaseTypeOfExpression<NotEqual> {
      typedef Expression<Transitive> VisitorBaseType;
    };
    /// Define the base type of less than expressions.
    template<>
    struct BaseTypeOfExpression<NotEqual> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        NotEqual,
        Relational,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for greater than or equal expressions.
    template<>
    struct VisitorBaseTypeOfExpression<GreaterThanOrEqual> {
      typedef Expression<Transitive> VisitorBaseType;
    };
    /// Define the base type of greater than expressions.
    template<>
    struct BaseTypeOfExpression<GreaterThanOrEqual> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        GreaterThanOrEqual,
        Relational,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for greater than expressions.
    template<>
    struct VisitorBaseTypeOfExpression<GreaterThan> {
      typedef Expression<Transitive> VisitorBaseType;
    };
    /// Define the base type of greater than expressions.
    template<>
    struct BaseTypeOfExpression<GreaterThan> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        GreaterThan,
        Relational,
        Transitive>::type BaseType;
    };
  }
}

#endif
