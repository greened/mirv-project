#ifndef mirv_core_ir_ExpressionFwd_hpp
#define mirv_core_ir_ExpressionFwd_hpp

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
  class Relational;
  class Ref;

  class Commutative;
  class Associative;
  class Transitive;
  class Reflexive;
}

#endif
