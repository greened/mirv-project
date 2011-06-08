#ifndef mirv_Core_IR_AddressConstant_hpp
#define mirv_Core_IR_AddressConstant_hpp

#include <mirv/Core/IR/Constant.hpp>

namespace mirv {
  struct SymbolVisitor;

  /// A symbol tag for specifying address constants.
  struct Address {};

  /// This represents a constant that is the address of some object.
  /// The object must be in a fixed memory location.
  template<>
  class Constant<Address> {
  private:
    class Interface
        : public Symbol<Constant<Base> >,
          public boost::enable_shared_from_this<Symbol<Constant<Address> > > {
    private:
      typedef Symbol<Constant<Base> > BaseType;
      typedef ptr<Symbol<Global> >::type SymbolPtr;
      SymbolPtr sym;

    public:
      Interface(ptr<Symbol<Type<TypeBase> > >::const_type type,
                SymbolPtr s)
          : BaseType(type), sym(s) {}

      SymbolPtr symbol(void) const {
        return sym;
      }

      std::string value(void) const {
        return "& " + sym->name();
      }

      ptr<Node<Base>>::type getSharedHandle(void) {
        return fast_cast<Node<Base>>(this->shared_from_this());
      };
      ptr<Node<Base>>::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(this->shared_from_this());
      };
    };

  public:
    static void
    initialize(typename ptr<Symbol<Constant<Address> > >::type constant) {}

    typedef Interface BaseType;
    typedef Symbol<Constant<Base> > VisitorBaseType;
  };    
}

#endif
