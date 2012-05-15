#ifndef mirv_Core_IR_PointerType_hpp
#define mirv_Core_IR_PointerType_hpp

#include <vector>

#include <mirv/Core/IR/PointerTypeFwd.hpp>
#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  struct Pointer;

  namespace detail {
    class PointerInterface : public Symbol<Type<Derived> >,
                             public boost::enable_shared_from_this<Symbol<Type<Pointer> > > {
    private:
      typedef Symbol<Type<Derived> > BaseType;

    public:
      PointerInterface(ptr<const Symbol<Type<TypeBase> > > base)
          : BaseType() {
        setBaseType(base);
      }
      typedef const Symbol<Type<TypeBase> > ChildType;
      typedef ptr<ChildType> ChildPtr;
      typedef ptr<const ChildType> ConstChildPtr;

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

      ptr<Node<Base>> getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      }
      ptr<const Node<Base>> getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(shared_from_this());
      }

      // FIXME: This is target-dependent.
      BitSizeType bitsize(void) const;
    };
  }

  /// A pointer type.
  struct Pointer {};
}

#endif
