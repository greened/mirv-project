#ifndef mirv_core_ir_statement_fwd_hh
#define mirv_core_ir_statement_fwd_hh

#include <mirv/ir/base.hh>
#include <mirv/ir/inherit.hh>

namespace mirv {
   template<
      typename Tag,
     typename Base = typename BaseType<Tag>::type>
   class Statement;
   typedef Statement<Base> BaseStatement;

  class InnerStatement;
  class LeafStatement;
   template<typename Stmt>
   class Controlled;
   template<typename Stmt>
   class SingleExpression;
}

#endif
