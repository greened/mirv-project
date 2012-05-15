#ifndef mirv_Core_IR_PlaceholderType_hpp
#define mirv_Core_IR_PlaceholderType_hpp

#include <mirv/Core/IR/PlaceholderTypeFwd.hpp>
#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/Type.hpp>

#include <boost/lexical_cast.hpp>

namespace mirv {
  namespace detail {
    class PlaceholderInterface : public Symbol<Type<TypeBase> >,
                                 public boost::enable_shared_from_this<Symbol<Type<Placeholder> > > {
    private:
      typedef Symbol<Type<TypeBase> > BaseType;

    public:
      BitSizeType bitsize(void) const;

      ptr<Node<Base>> getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      };
      ptr<const Node<Base>> getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(shared_from_this());
      };
    };
  }

  /// Placeholder types are used to stand in for another type during
  /// construction of recursive types.
   struct Placeholder {
   public:
     static std::string getName(void) {
       return "placeholder";
     }
     static std::string getName(ptr<Symbol<Module> >) {
       return getName();
     }
  };
}

#endif
