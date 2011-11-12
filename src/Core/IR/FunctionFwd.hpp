#ifndef mirv_core_ir_function_fwd_hh
#define mirv_core_ir_function_fwd_hh

#include <mirv/Core/IR/SymbolFwd.hpp>

namespace mirv {
  class Function;

  namespace detail {
  class FunctionInterface;

    template<>
    struct VisitorBaseTypeOfSymbol<Function> {
      typedef Symbol<Global> VisitorBaseType;
    };

    template<>
    struct BaseTypeOfSymbol<Function> {
      typedef FunctionInterface BaseType;
    };
  }
}

#endif

