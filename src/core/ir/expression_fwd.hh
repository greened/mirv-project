#ifndef mirv_core_ir_expression_fwd_hh
#define mirv_core_ir_expression_fwd_hh

namespace mirv {
  template<typename Op>
  class Expression;

  class InnerExpressionBase;
  class InnerExpression;
  class LeafExpression;
  class Unary;
  class Binary;

  class Arithmetic;
  class Logical;
  class Bitwise;
  class Ref;

  class Commutative;
  class Associative;
  class Transitive;
  class Reflexive;
}

#endif
