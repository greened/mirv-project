#include <mirv/Filter/Snapshot/Print/Print.hpp>

#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/IR/Producer.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Printer.hpp>

namespace mirv {
  void PrintFilter::runImpl(ptr<Module> node) {
    print(out, node);
  }
  void PrintFilter::runImpl(ptr<Function> node) {
    print(out, node);
  }
  void PrintFilter::runImpl(ptr<Control> node) {
    print(out, node);
  }
  void PrintFilter::runImpl(ptr<Producer> node) {
    print(out, node);
  }
}
