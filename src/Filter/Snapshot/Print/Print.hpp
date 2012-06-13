#ifndef mirv_Filter_Snapshot_Print_hpp
#define mirv_Filter_Snapshot_Print_hpp

#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <cstdint>

namespace mirv {
  /// This is a filter to print MIRV IR in textual form.
  class PrintFilter : public Filter<Node<Base> > {
  private:
    typedef Printer::Stream Stream;

    /// A handle to the output stream.
    Stream &out;

    void run(ptr<Node<Base> > node);
    void run(ptr<const Node<Base> > node);

  public:
    PrintFilter(Stream &o)
    : Filter<Node<Base> >(range(), range(), range()), out(o) {}
  };
}

#endif
