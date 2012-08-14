#ifndef mirv_Core_IR_Expression_hpp
#define mirv_Core_IR_Expression_hpp

#include <Core/IR/Node.hpp>
#include <Core/IR/Type.hpp>

#include <Core/IR/detail/IRNode.hpp>

namespace mirv {
  class Function;

  class Statement : public Node<Statement> {
  public:
    enum {
      IRKind = detail::IRNode::Statement;
    };

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

    Kind kind(void) const {
      return theKind;
    }

    private:
    Kind theKind;
    Index<Function> parentFunction;
    Index<IRNode> theParent;
  };
}

#endif
