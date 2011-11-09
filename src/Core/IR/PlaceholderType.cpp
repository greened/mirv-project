// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>
#include <mirv/Core/Builder/ExpressionGrammar.hpp>

#include <cstdint>

namespace mirv {
  detail::PlaceholderInterface::BitSizeType
  detail::PlaceholderInterface::bitsize(void) const
  {
    BitSizeType size =
      Builder::makeExpression(static_cast<std::uint64_t>(0ull),
                              this->parent<Symbol<Module> >());
    return size;
  }
}
