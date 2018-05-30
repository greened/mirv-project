#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <iostream>

namespace mirv {
  void Function::dump(void) {
    print(std::cout, getHandle(this));
  }
}
