#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Core/Utility/Printer.hpp>

namespace mirv {
  DebugManager * DebugManager::inst = 0;

  void dump(ptr<Node<Base> >::const_type node) 
  {
    print(std::cerr, node);
  }
}
