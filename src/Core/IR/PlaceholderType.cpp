#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  Placeholder::Interface::BitSizeType
  Placeholder::Interface::bitsize(void) const
  {
    BitSizeType size =
      Builder::makeExpression(0ull, this->parent<Symbol<Module> >());
    return size;
  }
}
