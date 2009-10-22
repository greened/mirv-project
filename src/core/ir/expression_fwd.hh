#ifndef mirv_core_ir_expression_fwd_hh
#define mirv_core_ir_expression_fwd_hh

#include <mirv/ir/base.hh>
#include <mirv/ir/inherit.hh>

namespace mirv {
   template<
      typename Op,
     typename Base = typename BaseType<Op>::type>
   class Expression;
   typedef Expression<Base> BaseExpression;

  class InnerExpression;
  class LeafExpression;
  class Unary;
  class Binary;
}

#endif
