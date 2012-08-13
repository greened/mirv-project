#ifndef mirv_Core_IR_Expression_hpp
#define mirv_Core_IR_Expression_hpp

#include <Core/IR/Node.hpp>
#include <Core/IR/Type.hpp>

namespace mirv {
  class Function;

  class Statement : public Node<Statement> {
  public:
    enum Kind {
      Null,

      // Control
      Block,
      IfThen,
      IfElse,
      DoWhile,
      Case,
      CaseBlock,
      Switch,
      Before,
      After,
      Goto,
      Return,

      // State modification
      Phi,
      Store,
      Call,
      Allocate
    };

    typedef KindTuple<Kind,
                      Block, IfThen, IfElse, DoWhile, Case, CaseBlock, Switch,
                      Before, After, Goto, Return,
                      Phi, Store, Call, Allocate> Kinds;

    template<Kind K>
    class VisitKinds {
    public:
      typedef KindTuple<> type;
    }:

    private:
    Index<Function> parentFunction;
  };
}

#endif
