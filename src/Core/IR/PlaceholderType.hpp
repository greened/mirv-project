#ifndef mirv_Core_IR_PlaceholderType_hpp
#define mirv_Core_IR_PlaceholderType_hpp

#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/Type.hpp>

#include <boost/lexical_cast.hpp>

namespace mirv {
  /// Placeholder types are used to stand in for another type during
  /// construction of recursive types.
   struct Placeholder {
   private:
    typedef Symbol<Type<TypeBase> > InterfaceBaseType;

     class Interface : public InterfaceBaseType,
                       public boost::enable_shared_from_this<Symbol<Type<Placeholder> > > {
     private:
       typedef InterfaceBaseType BaseType;

     public:
       BitSizeType bitsize(void) const;

       ptr<Node<Base>>::type getSharedHandle(void) {
         return fast_cast<Node<Base>>(shared_from_this());
       };
       ptr<Node<Base>>::const_type getSharedHandle(void) const {
         return fast_cast<const Node<Base>>(shared_from_this());
       };
     };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<TypeBase> > VisitorBaseType;

     static std::string getName(void) {
       return "placeholder";
     }
     static std::string getName(ptr<Symbol<Module> >::type) {
       return getName();
     }
  };
}

#endif
