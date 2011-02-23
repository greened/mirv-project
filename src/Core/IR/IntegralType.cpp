#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
// FIXME: Why is this necessary?
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  Integral::Interface::Interface(std::uint64_t size)
      : InterfaceBaseType("int" + boost::lexical_cast<std::string>(size),
                          size)
  {
  }
}
