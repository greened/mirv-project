#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  detail::IntegralInterface::IntegralInterface(std::uint64_t size)
      : BaseType(size)
  {
  }
}
