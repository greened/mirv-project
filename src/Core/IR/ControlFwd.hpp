#ifndef mirv_core_ir_control_fwd_hh
#define mirv_core_ir_control_fwd_hh

#include <mirv/Core/IR/StatementFwd.hpp>

namespace mirv {
  class Block;
  class SingleBlock;
  class DualBlock;
  class SingleCondition; 
  class IfThen;
  class IfElse;
  class While;
  class DoWhile;
  class Case;
  class CaseBlock;
  class Switch;
  class SingleLabel;
  class Before;
  class After;
  class Goto;
  class Return;

  namespace detail {
    class BlockInterface;

    /// Define the visitation base type for block statements.
    template<>
    struct VisitorBaseTypeOfStatement<Block> {
      typedef InnerStatement VisitorBaseType;
    };
    /// Define the base type of block statements.
    template<>
    struct BaseTypeOfStatement<Block> {
      typedef BlockInterface BaseType;
    };

    class SingleBlockInterface;

    /// Define the visitation base type for single block statements.
    template<>
    struct VisitorBaseTypeOfStatement<SingleBlock> {
      typedef InnerStatement VisitorBaseType;
    };
    /// Define the base type of single block statements.
    template<>
    struct BaseTypeOfStatement<SingleBlock> {
      typedef SingleBlockInterface BaseType;
    };

    class DualBlockInterface;

    /// Define the visitation base type for dual block statements.
    template<>
    struct VisitorBaseTypeOfStatement<DualBlock> {
      typedef InnerStatement VisitorBaseType;
    };
    /// Define the base type of dual block statements.
    template<>
    struct BaseTypeOfStatement<DualBlock> {
      typedef DualBlockInterface BaseType;
    };

    class SingleConditionInterface;

    /// Define the visitation base type for single condition statements.
    template<>
    struct VisitorBaseTypeOfStatement<SingleCondition> {
      typedef Statement<SingleExpression> VisitorBaseType;
    };
    /// Define the base type of single condition statements.
    template<>
    struct BaseTypeOfStatement<SingleCondition> {
      typedef SingleConditionInterface BaseType;
    };

    class IfThenInterface;

    /// Define the visitation base type for if-then statements.
    template<>
    struct VisitorBaseTypeOfStatement<IfThen> {
      typedef Statement<SingleBlock> VisitorBaseType;
    };
    /// Define the base type of single if-then statements.
    template<>
    struct BaseTypeOfStatement<IfThen> {
      typedef StatementBaseGenerator<IfThenInterface, IfThen, Conditional>::type BaseType;
    };

    class IfElseInterface;

    /// Define the visitation base type for if-else statements.
    template<>
    struct VisitorBaseTypeOfStatement<IfElse> {
      typedef Statement<DualBlock> VisitorBaseType;
    };
    /// Define the base type of single if-else statements.
    template<>
    struct BaseTypeOfStatement<IfElse> {
      typedef StatementBaseGenerator<IfElseInterface, IfElse, Conditional>::type BaseType;
    };

    class WhileInterface;

    /// Define the visitation base type for while statements.
    template<>
    struct VisitorBaseTypeOfStatement<While> {
      typedef Statement<SingleBlock> VisitorBaseType;
    };
    /// Define the base type of single while statements.
    template<>
    struct BaseTypeOfStatement<While> {
      typedef StatementBaseGenerator<WhileInterface, While, Conditional, Iterative>::type BaseType;
    };

    class DoWhileInterface;

    /// Define the visitation base type for do-while statements.
    template<>
    struct VisitorBaseTypeOfStatement<DoWhile> {
      typedef Statement<SingleBlock> VisitorBaseType;
    };
    /// Define the base type of single do-while statements.
    template<>
    struct BaseTypeOfStatement<DoWhile> {
      typedef StatementBaseGenerator<DoWhileInterface, DoWhile, Conditional>::type BaseType;
    };

    class CaseInterface;

    /// Define the visitation base type for case statements.
    template<>
    struct VisitorBaseTypeOfStatement<Case> {
      typedef Statement<SingleBlock> VisitorBaseType;
    };
    /// Define the base type of single case statements.
    template<>
    struct BaseTypeOfStatement<Case> {
      typedef CaseInterface BaseType;
    };

    class CaseBlockInterface;

    /// Define the visitation base type for case block statements.
    template<>
    struct VisitorBaseTypeOfStatement<CaseBlock> {
      typedef Statement<SingleBlock> VisitorBaseType;
    };
    /// Define the base type of case block statements.
    template<>
    struct BaseTypeOfStatement<CaseBlock> {
      typedef CaseBlockInterface BaseType;
    };

    class SwitchInterface;

    /// Define the visitation base type for switch statements.
    template<>
    struct VisitorBaseTypeOfStatement<Switch> {
      typedef Statement<SingleBlock> VisitorBaseType;
    };
    /// Define the base type of switch statements.
    template<>
    struct BaseTypeOfStatement<Switch> {
      typedef StatementBaseGenerator<SwitchInterface, Switch, Conditional>::type BaseType;
    };

    class SingleLabelInterface;

    /// Define the visitation base type for single label statements.
    template<>
    struct VisitorBaseTypeOfStatement<SingleLabel> {
      typedef Statement<SingleExpression> VisitorBaseType;      
    };
    /// Define the base type of single single label statements.
    template<>
    struct BaseTypeOfStatement<SingleLabel> {
      typedef SingleLabelInterface BaseType;
    };

    class BeforeInterface;

    /// Define the visitation base type for before statements.
    template<>
    struct VisitorBaseTypeOfStatement<Before> {
      typedef Statement<SingleBlock> VisitorBaseType;      
    };
    /// Define the base type of single before statements.
    template<>
    struct BaseTypeOfStatement<Before> {
      typedef StatementBaseGenerator<BeforeInterface, Before, Iterative>::type BaseType;
    };

    class AfterInterface;

    /// Define the visitation base type for after statements.
    template<>
    struct VisitorBaseTypeOfStatement<After> {
      typedef Statement<SingleBlock> VisitorBaseType;      
    };
    /// Define the base type of single after statements.
    template<>
    struct BaseTypeOfStatement<After> {
      typedef StatementBaseGenerator<AfterInterface, After, Conditional>::type BaseType;
    };

    class GotoInterface;

    /// Define the visitation base type for goto statements.
    template<>
    struct VisitorBaseTypeOfStatement<Goto> {
      typedef LeafStatement VisitorBaseType;
    };
    /// Define the base type of single goto statements.
    template<>
    struct BaseTypeOfStatement<Goto> {
      typedef StatementBaseGenerator<GotoInterface, Goto, Conditional>::type BaseType;
    };

    class ReturnInterface;

    /// Define the visitation base type for return statements.
    template<>
    struct VisitorBaseTypeOfStatement<Return> {
      typedef LeafStatement VisitorBaseType;
    };
    /// Define the base type of single return statements.
    template<>
    struct BaseTypeOfStatement<Return> {
      typedef ReturnInterface BaseType;
    };
  }
};

#endif
