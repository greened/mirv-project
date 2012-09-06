#ifndef mirv_Core_IR_Expression_hpp
#define mirv_Core_IR_Expression_hpp

#include <Core/IR/Node.hpp>
#include <Core/IR/Type.hpp>

#include <Core/IR/detail/IRNode.hpp>

namespace mirv {
  class Function;

  class Statement : public Node<Statement> {
  private:
    template<typename T>
    class ChildrenDispatcher {
    public:
      static Range dispatch(Statement node) {
        return node.childrenImpl<T>();
      }
    };

    friend class ChiildrenDispatcher;

    template<typename K>
    Range childrenImpl(void) {
      return Node<Statement>::children();
    }

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

    template<typename T = Statement>
    Range children(void) {
      Dispatch<Range, ChildrenDispatcher<T>, Kinds>::
        dispatch(*this, this->theKind);
    }

    template<typename T = Statement>
    ConstRange children(void) const {
      return Node<Statement>::begin<T>();
    }

  private:
    Kind theKind;
    Index<Function> parentFunction;
    Index<IRNode> theParent;
  };
}

#endif
