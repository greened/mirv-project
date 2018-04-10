#ifndef mirv_Core_IR_AddressConstant_hpp
#define mirv_Core_IR_AddressConstant_hpp

#include <mirv/Core/IR/AddressConstantFwd.hpp>
#include <mirv/Core/IR/Constant.hpp>

namespace mirv {
  struct SymbolVisitor;

  /// A symbol tag for specifying address constants.
  struct Address {};

  namespace detail {
    class AddressConstantInterface
        : public Symbol<Constant<Base> >,
          public boost::enable_shared_from_this<Symbol<Constant<Address> > > {
    private:
      typedef Symbol<Constant<Base> > BaseType;
      typedef ptr<Symbol<Global> > SymbolPtr;
      SymbolPtr sym;

    public:
      AddressConstantInterface(ptr<const Symbol<Type<TypeBase> > > type,
                               SymbolPtr s)
          : BaseType(type), sym(s) {}

      SymbolPtr symbol(void) const {
        return sym;
      }

      std::string value(void) const {
        return "& " + sym->name();
      }

      ptr<Node<Base>> getSharedHandle(void) {
        return fast_cast<Node<Base>>(this->shared_from_this());
      };
      ptr<const Node<Base>> getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(this->shared_from_this());
      };
    };
  }

  /// This represents a constant that is the address of some object.
  /// The object must be in a fixed memory location.
  template<>
  class Constant<Address> {
  public:
    static void
    initialize(ptr<Symbol<Constant<Address> > > constant) {}
  };
}

#endif
