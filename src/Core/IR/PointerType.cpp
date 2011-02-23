#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/PointerType.hpp>
// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  Pointer::Interface::BitSizeType
  Pointer::Interface::bitsize(void) const 
  {
    // FIXME: This is target-dependent.
    return Builder::makeExpression(static_cast<std::uint64_t>(64),
                                   getBaseType()->parent<Symbol<Module> >());
  }
}
