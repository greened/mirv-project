#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/Type.hpp>
// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  Simple::Interface::BitSizeType
  Simple::Interface::bitsize(void) const
  {
    BitSizeType size =
      Builder::makeExpression(bsize, this->parent<Symbol<Module> >());
    return size;
  }
}
