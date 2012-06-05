#include <mirv/Filter/Snapshot/Print/Print.hpp>

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Printer.hpp>

namespace mirv {
  void PrintFilter::run(ptr<Node<Base> > node)
  {
    run(boost::const_pointer_cast<const Node<Base> >(node));
  }

  void PrintFilter::run(ptr<const Node<Base> > node)
  {
    print(out, node);
  }
}
