#ifndef mirv_Core_Utility_Printer_hpp
#define mirv_Core_Utility_Printer_hpp

#include <mirv/Core/IR/Node.hpp>

#include <cstdint>

namespace mirv {
  /// This is a filter to print MIRV IR in textual form.
  namespace Printer {
    typedef std::ostream Stream;
  }
  void print(Printer::Stream &out, ptr<Node<Base> >::const_type node);
  void printWithTypes(Printer::Stream &out, ptr<Node<Base> >::const_type node);
}

#endif
