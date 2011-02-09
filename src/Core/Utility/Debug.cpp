#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

namespace mirv {
  DebugManager * DebugManager::inst = 0;

  void dump(ptr<Node<Base> >::const_type node) 
  {
    //PrintFilter print(std::cerr);
    //print(node);
  }
}
