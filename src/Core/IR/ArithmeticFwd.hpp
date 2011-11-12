#ifndef mirv_core_ir_arithmetic_fwd_hh
#define mirv_core_ir_arithmetic_fwd_hh

#include <mirv/Core/IR/ExpressionFwd.hpp>

namespace mirv {
  // Expression Ops
  class Add;
  class Subtract;
  class Multiply;
  class Divide;
  class Modulus;
  class Negate;

  namespace detail {
    /// Define the visitation base type for add expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Add> {
      typedef Expression<Commutative> VisitorBaseType;
    };
    /// Define the base type of add expressions.
    template<>
    struct BaseTypeOfExpression<Add> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        Add,
        Arithmetic,
        Associative,
        Commutative,
        Reflexive,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for subtract expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Subtract> {
      typedef Expression<Associative> VisitorBaseType;
    };
    /// Define the base type of subtract expressions.
    template<>
    struct BaseTypeOfExpression<Subtract> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        Subtract,
        Arithmetic,
        Associative,
        Reflexive,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for multiply expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Multiply> {
      typedef Expression<Commutative> VisitorBaseType;
    };
    /// Define the base type of multiply expressions.
    template<>
    struct BaseTypeOfExpression<Multiply> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        Multiply,
        Arithmetic,
        Associative,
        Commutative,
        Reflexive,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for divide expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Divide> {
      typedef Expression<Transitive> VisitorBaseType;
    };
    /// Define the base type of divide expressions.
    template<>
    struct BaseTypeOfExpression<Divide> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        Divide,
        Arithmetic,
        Reflexive,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for modulus expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Modulus> {
      typedef Expression<Transitive> VisitorBaseType;
    };
    /// Define the base type of modulus expressions.
    template<>
    struct BaseTypeOfExpression<Modulus> {
      typedef ExpressionBaseGenerator<
        Expression<Binary>,
        Modulus,
        Arithmetic,
        Reflexive,
        Transitive>::type BaseType;
    };

    /// Define the visitation base type for negate expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Negate> {
      typedef Expression<Reflexive> VisitorBaseType;
    };
    /// Define the base type of negate expressions.
    template<>
    struct BaseTypeOfExpression<Negate> {
      typedef ExpressionBaseGenerator<
        Expression<Unary>,
        Negate,
        Arithmetic,
        Reflexive>::type BaseType;
    };
  }
};

#endif
