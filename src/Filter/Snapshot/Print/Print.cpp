#include <mirv/Filter/Snapshot/Print/Print.hpp>

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Printer.hpp>

namespace mirv {
  void PrintFilter::operator()(ptr<Node<Base> >::const_type node)
  {
    print(out, node);
  }
}
