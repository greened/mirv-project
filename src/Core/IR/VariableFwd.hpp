#ifndef mirv_core_ir_variable_fwd_hh
#define mirv_core_ir_variable_fwd_hh

#include <mirv/Core/IR/SymbolFwd.hpp>

namespace mirv {
  class Variable;

  namespace detail {
    class VariableInterface;

    template<>
    struct VisitorBaseTypeOfSymbol<Variable> {
      typedef LeafSymbol VisitorBaseType;
    };

    template<>
    struct BaseTypeOfSymbol<Variable> {
      typedef VariableInterface BaseType;
    };
  }
}

#endif
