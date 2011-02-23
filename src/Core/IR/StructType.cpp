#include <mirv/Core/Builder/AddOperators.hpp>
#include <mirv/Core/IR/StructType.hpp>
// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  StructType::Interface::BitSizeType
  StructType::Interface::bitsize(void) const 
  {
    // This assumes all padding is in place.
    BitSizeType expr =
      Builder::makeExpression(0, this->parent<Symbol<Module> >());
    for (auto m = memberBegin(); m != memberEnd(); ++m) {
      expr = expr + (*m)->bitsize();
    }
    return expr;
  }
}
