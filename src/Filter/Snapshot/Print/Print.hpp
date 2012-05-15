#ifndef mirv_Filter_Snapshot_Print_hpp
#define mirv_Filter_Snapshot_Print_hpp

#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <cstdint>

namespace mirv {
  /// This is a filter to print MIRV IR in textual form.
  class PrintFilter : public ConstFilter<Node<Base> > {
  private:
    typedef Printer::Stream Stream;

    /// A handle to the output stream.
    Stream &out;

  public:
    PrintFilter(Stream &o) : out(o) {}

    void operator()(ptr<const Node<Base> > node);
  };
}

#endif
