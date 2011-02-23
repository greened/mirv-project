#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/FloatingType.hpp>

namespace mirv {
  Floating::Interface::Interface(std::uint64_t size)
      : InterfaceBaseType("float" + boost::lexical_cast<std::string>(size),
                          size)
  {
  }
}
