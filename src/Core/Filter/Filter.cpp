#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/IR/Module.hpp>

namespace mirv {
  FilterBase::~FilterBase(void) {}

  void Filter::runImpl(ptr<Module> M) {
    for (auto F : M->Functions()) {
      run(F);
    }
  }
}
