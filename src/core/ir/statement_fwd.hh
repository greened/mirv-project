#ifndef mirv_core_ir_statement_fwd_hh
#define mirv_core_ir_statement_fwd_hh

namespace mirv {
  template<typename Tag> class Statement;

  class InnerStatement;
  class LeafStatement;

  class Conditional;
  class Iterative;
  class Mutating;

  class Controlled;
  class SingleExpression;
}

#endif
