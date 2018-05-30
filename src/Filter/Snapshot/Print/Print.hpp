#ifndef mirv_Filter_Snapshot_Print_hpp
#define mirv_Filter_Snapshot_Print_hpp

#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Utility/Printer.hpp>

#include <cstdint>

namespace mirv {
  class Control;
  class Function;
  class Module;
  class Producer;
  class Symbol;

  /// This is a filter to print MIRV IR in textual form.
  class PrintFilter : public Filter {
  private:
    typedef Printer::Stream Stream;

    /// A handle to the output stream.
    Stream &out;

    void runImpl(ptr<Module> node) override;
    void runImpl(ptr<Function> node) override;
    void runImpl(ptr<Control> node) override;
    void runImpl(ptr<Producer> node) override;

  public:
    PrintFilter(Stream &o) : Filter(range(), range(), range()), out(o) {}
  };
}

#endif
