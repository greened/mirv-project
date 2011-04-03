// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/Builder/ExpressionGrammar.hpp>

#include <cstdint>

namespace mirv {
  Placeholder::Interface::BitSizeType
  Placeholder::Interface::bitsize(void) const
  {
    BitSizeType size =
      Builder::makeExpression(static_cast<std::uint64_t>(0ull),
                              this->parent<Symbol<Module> >());
    return size;
  }
}
