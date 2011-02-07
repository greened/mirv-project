#ifndef mirv_Core_IR_PointerType_hpp
#define mirv_Core_IR_PointerType_hpp

#include <vector>

#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  /// A pointer type.
  struct Pointer {
  private:
    typedef Symbol<Type<Derived> > InterfaceBaseType;

    class Interface : public InterfaceBaseType,
                      public boost::enable_shared_from_this<Symbol<Type<Pointer> > > {
    public:
      Interface(ptr<Symbol<Type<TypeBase>>>::const_type base)
          : InterfaceBaseType(base->name() + "*") {
        setBaseType(base);
      }
      typedef const Symbol<Type<TypeBase> > ChildType;
      typedef ptr<ChildType>::type ChildPtr;
      typedef ptr<ChildType>::const_type ConstChildPtr;

      void setBaseType(ChildPtr c) {
        if (empty()) {
          push_back(c);
        }
        else {
          *begin() = c;
        }
      }

      ChildPtr getBaseType(void) {
        return(front());
      }

      ConstChildPtr getBaseType(void) const {
        return(front());
      }

      ptr<Node<Base>>::type getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      }

      // FIXME: This is target-dependent.
      BitSizeType bitsize(void) const {
        return 64;
      }
    };

  public:
    typedef Interface BaseType;
    typedef Symbol<Type<Derived> > VisitorBaseType;

    static std::string getName(ptr<Symbol<Type<TypeBase>>>::const_type base) {
      return base->name() + "*";
    }
  };
}

#endif
