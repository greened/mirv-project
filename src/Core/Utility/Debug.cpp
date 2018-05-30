#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Core/Utility/Printer.hpp>

namespace mirv {
  DebugManager * DebugManager::inst = 0;

  void dump(ptr<const Type> symbol) {
    print(std::cerr, symbol);
  }

  void dump(ptr<Control> statement) {
    print(std::cerr, statement);
  }

  void dump(ptr<Producer> expression) {
    print(std::cerr, expression);
  }
}
