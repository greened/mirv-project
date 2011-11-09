#ifndef mirv_Core_IR_GlobalVariableFwd_hpp
#define mirv_Core_IR_GlobalVariableFwd_hpp

namespace mirv {
  class GlobalVariable;

  namespace detail {
    class GlobalVariableInterface;

    template<>
    struct VisitorBaseTypeOfSymbol<GlobalVariable> {
      typedef LeafSymbol VisitorBaseType;
    };

    template<>
    struct BaseTypeOfSymbol<GlobalVariable> {
      typedef GlobalVariableInterface BaseType;
    };
  }
}

#endif
