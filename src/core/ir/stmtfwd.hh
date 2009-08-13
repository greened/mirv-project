#ifndef MIRV_Core_IR_Stmtfwd_hh
#define MIRV_Core_IR_Stmtfwd_hh

namespace MIRV {
   // Tags
   class Conditional;
   class Iterative;
   class Mutating;

   // Control
   class Block;
   class IfThen;
   class IfElse;
   class While;
   class DoWhile;
   class Case;
   class Switch;
   class Before;
   class After;
   class Goto;
   class Return;
   class Label;

   // Mutating
   class Assignment;
}

#endif
