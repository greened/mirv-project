#ifndef mirv_core_ir_logical_fwd_hh
#define mirv_core_ir_logical_fwd_hh

namespace mirv {
  class LogicalAnd;
  class LogicalOr;
  class LogicalNot;

  namespace detail {
    /// Define the visitation base type for and expressions.
    template<>
    struct VisitorBaseTypeOfExpression<LogicalAnd> {
      typedef Expression<Commutative> VisitorBaseType;
    };
    /// Define the base type of and expressions.
    template<>
    struct BaseTypeOfExpression<LogicalAnd> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        LogicalAnd,
        Associative,
        Commutative,
        Logical,
        Reflexive,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for or expressions.
    template<>
    struct VisitorBaseTypeOfExpression<LogicalOr> {
      typedef Expression<Commutative> VisitorBaseType;
    };
    /// Define the base type of or expressions.
    template<>
    struct BaseTypeOfExpression<LogicalOr> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        LogicalOr,
        Associative,
        Commutative,
        Logical,
        Reflexive,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for not expressions.
    template<>
    struct VisitorBaseTypeOfExpression<LogicalNot> {
      typedef Expression<Reflexive> VisitorBaseType;
    };
    /// Define the base type of not expressions.
    template<>
    struct BaseTypeOfExpression<LogicalNot> {
      typedef ExpressionBaseGenerator<
        Expression<Unary>,
        LogicalNot,
        Logical,
        Reflexive>::type BaseType;
    };
  }
};

#endif
