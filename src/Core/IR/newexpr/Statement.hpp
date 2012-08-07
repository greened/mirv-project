#ifndef mirv_Core_IR_Expression_hpp
#define mirv_Core_IR_Expression_hpp

#include <Core/IR/Node.hpp>
#include <Core/IR/Type.hpp>

namespace mirv {
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

  private:
    Index<Type> theType;
  };
}

#endif
