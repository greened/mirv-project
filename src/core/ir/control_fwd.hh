#ifndef mirv_core_ir_control_fwd_hh
#define mirv_core_ir_control_fwd_hh

#include <mirv/ir/statement_fwd.hh>

namespace mirv {
  class Block;
   template<typename Base>
   class SingleBlock;
   template<typename Base>
   class DualBlock;
   template<typename Stmt>
   class SingleCondition;
  class IfThen;
  class IfElse;
  class While;
  class DoWhile;
  class Case;
  class CaseBlock;
  class Switch;
   template<typename Stmt>
   class SingleLabel;
  class Before;
  class After;
  class Goto;
  class Return;
};

#endif
