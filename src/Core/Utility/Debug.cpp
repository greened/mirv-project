#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Core/Utility/Printer.hpp>

namespace mirv {
  DebugManager * DebugManager::inst = 0;

  void dump(ptr<const Node<Base> > node) 
  {
    print(std::cerr, node);
  }
}
