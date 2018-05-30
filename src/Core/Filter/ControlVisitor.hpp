#ifndef mirv_Core_Filter_ControlVisitor_hpp
#define mirv_Core_Filter_ControlVisitor_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Library/TypeList.hpp>

namespace mirv {
  class Conrol;
  class ValueControl;
  class Block;
  class CIBlock;
  class Goto;
  class Conditional;
  class IfThen;
  class IfElse;
  class DoWhile;
  class Case;
  class Switch;
  class Sequence;
  class Return;
  class SubroutineCall;
  class PureSubroutineCall;

  /// This is the base class for all visitors of statements.
  class ControlVisitor {
  public:
    virtual ~ControlVisitor(void) = default;

    virtual void visit(Control &);
    virtual void visit(ValueControl &);
    virtual void visit(Block &);
    virtual void visit(CIBlock &);
    virtual void visit(IfThen &);
    virtual void visit(IfElse &);
    virtual void visit(DoWhile &);
    virtual void visit(Switch &);
    virtual void visit(Case &);
    virtual void visit(Goto &);
    virtual void visit(Sequence &);
    virtual void visit(Return &);
    virtual void visit(SubroutineCall &);
    virtual void visit(PureSubroutineCall &);
  };
}

#endif
