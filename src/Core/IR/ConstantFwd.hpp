#ifndef mirv_core_ir_constant_fwd_hh
#define mirv_core_ir_constant_fwd_hh

#include <mirv/Core/IR/SymbolFwd.hpp>

namespace mirv {
  template<typename ValueType>
  class Constant;

  namespace detail {
    class BaseConstantInterface;
    template<typename ValueType>
    class ConstantInterface;

    template<>
    struct VisitorBaseTypeOfSymbol<Constant<Base> > {
      typedef LeafSymbol VisitorBaseType;
    };

    template<>
    struct BaseTypeOfSymbol<Constant<Base> > {
      typedef detail::BaseConstantInterface BaseType;
    };

    template<typename ValueType>
    struct VisitorBaseTypeOfSymbol<Constant<ValueType> > {
      typedef Symbol<Constant<Base> > VisitorBaseType;
    };

    template<typename ValueType>
    struct BaseTypeOfSymbol<Constant<ValueType> > {
      typedef detail::ConstantInterface<ValueType> BaseType;
    };
  }
}

#endif
