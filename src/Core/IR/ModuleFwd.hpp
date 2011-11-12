#ifndef mirv_core_ir_module_fwd_hh
#define mirv_core_ir_module_fwd_hh

#include <mirv/Core/IR/SymbolFwd.hpp>

namespace mirv {
  class Module;

  namespace detail {
    class ModuleInterface;

    template<>
    struct VisitorBaseTypeOfSymbol<Module> {
      typedef Symbol<Named> VisitorBaseType;
    };

    template<>
    struct BaseTypeOfSymbol<Module> {
      typedef ModuleInterface BaseType;
    };
  }
}

#endif
