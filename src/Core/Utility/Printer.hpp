#ifndef mirv_Core_Utility_Printer_hpp
#define mirv_Core_Utility_Printer_hpp

#include <mirv/Core/Memory/Heap.hpp>

#include <cstdint>
#include <ostream>

namespace mirv {
  class Control;
  class Function;
  class Module;
  class Producer;
  class Type;

  /// This is a filter to print MIRV IR in textual form.
  namespace Printer {
    typedef std::ostream Stream;
  }
  void print(Printer::Stream &out, ptr<Module> statement);
  void print(Printer::Stream &out, ptr<Function> statement);
  void print(Printer::Stream &out, ptr<const Type> symbol);
  void print(Printer::Stream &out, ptr<Control> statement);
  void print(Printer::Stream &out, ptr<Producer> expression);

  void printWithTypes(Printer::Stream &out, ptr<Module> statement);
  void printWithTypes(Printer::Stream &out, ptr<Function> statement);
  void printWithTypes(Printer::Stream &out, ptr<const Type> symbol);
  void printWithTypes(Printer::Stream &out, ptr<Control> statement);
  void printWithTypes(Printer::Stream &out, ptr<Producer> expression);
}

#endif
