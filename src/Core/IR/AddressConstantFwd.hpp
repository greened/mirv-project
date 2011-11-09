#ifndef mirv_Core_IR_AddressConstantFwd_hpp
#define mirv_Core_IR_AddressConstantFwd_hpp

#include <mirv/Core/IR/ConstantFwd.hpp>

namespace mirv {
  struct Address;

  template<>
  class Constant<Address>;

  namespace detail {
    class AddressConstantInterface;

    template<>
    struct VisitorBaseTypeOfSymbol<Constant<Address> > {
      typedef Symbol<Constant<Base> > VisitorBaseType;
    };

    template<>
    struct BaseTypeOfSymbol<Constant<Address> > {
      typedef AddressConstantInterface BaseType;
    };
  }
}

#endif
