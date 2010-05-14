#ifndef mirv_Core_IR_PointerType_hpp
#define mirv_Core_IR_PointerType_hpp

#include <vector>

#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  /// A pointer type.
  struct Pointer {
  private:
    typedef Symbol<Type<Derived> > InterfaceBaseType;

    class Interface : public InterfaceBaseType {
    public:
      Interface(const std::string &name) : InterfaceBaseType(name) {}
      typedef Symbol<Type<TypeBase> > ChildType;
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
      virtual void accept(mirv::SymbolVisitor &) {
        error("Pointer::accept called");
      }
    };

  public:
    typedef Interface BaseType;
    typedef Symbol<Type<Derived> > VisitorBaseType;
  };
}

#endif
