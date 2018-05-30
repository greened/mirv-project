#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/Utility/Printer.hpp>

namespace mirv {
  void Module::dump(void) {
    print(std::cout, getHandle(this));
  }
}
