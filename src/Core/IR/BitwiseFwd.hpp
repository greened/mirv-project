#ifndef mirv_core_ir_bitwise_fwd_hh
#define mirv_core_ir_bitwise_fwd_hh

namespace mirv {
  class BitwiseAnd;
  class BitwiseOr;
  class BitwiseXor;
  class BitwiseComplement;
  class ShiftLeft;
  class ArithmeticShiftRight;
  class LogicalShiftRight;

  namespace detail {
    /// Define the visitation base type for and expressions.
    template<>
    struct VisitorBaseTypeOfExpression<BitwiseAnd> {
      typedef Expression<Commutative> VisitorBaseType;
    };
    /// Define the base type of and expressions.
    template<>
    struct BaseTypeOfExpression<BitwiseAnd> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        BitwiseAnd,
        Associative,
        Bitwise,
        Commutative,
        Reflexive,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for or expressions.
    template<>
    struct VisitorBaseTypeOfExpression<BitwiseOr> {
      typedef Expression<Commutative> VisitorBaseType;
    };
    /// Define the base type of or expressions.
    template<>
    struct BaseTypeOfExpression<BitwiseOr> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        BitwiseOr,
        Associative,
        Bitwise,
        Commutative,
        Reflexive,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for xor expressions.
    template<>
    struct VisitorBaseTypeOfExpression<BitwiseXor> {
      typedef Expression<Commutative> VisitorBaseType;
    };
    /// Define the base type of xor expressions.
    template<>
    struct BaseTypeOfExpression<BitwiseXor> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        BitwiseXor,
        Associative,
        Bitwise,
        Commutative,
        Reflexive,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for complement expressions.
    template<>
    struct VisitorBaseTypeOfExpression<BitwiseComplement> {
      typedef Expression<Reflexive> VisitorBaseType;
    };
    /// Define the base type of complement expressions.
    template<>
    struct BaseTypeOfExpression<BitwiseComplement> {
      typedef ExpressionBaseGenerator<
        Expression<Unary>,
        BitwiseComplement,
        Bitwise,
        Reflexive>::type BaseType;
    };

    /// Define the visitation base type for shl expressions.
    template<>
    struct VisitorBaseTypeOfExpression<ShiftLeft> {
      typedef Expression<Binary> VisitorBaseType;
    };
    /// Define the base type of shl expressions.
    template<>
    struct BaseTypeOfExpression<ShiftLeft> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        ShiftLeft,
        Bitwise>::type BaseType;
    };

    /// Define the visitation base type for ashr expressions.
    template<>
    struct VisitorBaseTypeOfExpression<ArithmeticShiftRight> {
      typedef Expression<Binary> VisitorBaseType;
    };
    /// Define the base type of ashr expressions.
    template<>
    struct BaseTypeOfExpression<ArithmeticShiftRight> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        ArithmeticShiftRight,
        Bitwise>::type BaseType;
    };

    /// Define the visitation base type for lshr expressions.
    template<>
    struct VisitorBaseTypeOfExpression<LogicalShiftRight> {
      typedef Expression<Binary> VisitorBaseType;
    };
    /// Define the base type of lshr expressions.
    template<>
    struct BaseTypeOfExpression<LogicalShiftRight> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        LogicalShiftRight,
        Bitwise>::type BaseType;
    };
  }
};

#endif
