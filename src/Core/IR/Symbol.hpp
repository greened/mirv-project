#ifndef mirv_Core_IR_Symbol_hpp
#define mirv_Core_IR_Symbol_hpp

#include <mirv/Core/IR/Attributable.hpp>
#include <mirv/Core/IR/NodeName.hpp>
#include <mirv/Core/IR/NodeType.hpp>
#include <mirv/Core/Utility/Printer.hpp>
#include <mirv/Library/TypeList.hpp>

#include <functional>
#include <string>
#include <sstream>

namespace mirv {
  class Type;

  struct SymbolVisitor;
  struct ConstSymbolVisitor;

  class Symbol : public Attributable {
    virtual void acceptImpl(SymbolVisitor &) = 0;

  protected:
    Symbol(void) {}

  public:
    using VisitorBaseTypes = TypeList<>;

    void accept(SymbolVisitor &V) {
      acceptImpl(V);
    }
  };
}

#endif
