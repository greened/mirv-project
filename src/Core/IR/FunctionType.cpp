#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/FunctionType.hpp>
// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  FunctionType::Interface::BitSizeType
  FunctionType::Interface::bitsize(void) const 
  {
    // FIXME: This is target-dependent.
    return Builder::makeExpression(static_cast<std::uint64_t>(0),
                                   getReturnType()->parent<Symbol<Module> >());
  }
}
