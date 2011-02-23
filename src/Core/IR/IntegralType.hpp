#ifndef mirv_Core_IR_IntegralType_hpp
#define mirv_Core_IR_IntegralType_hpp

#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/Type.hpp>

#include <boost/lexical_cast.hpp>

namespace mirv {
  /// Integer types.
   struct Integral {
   private:
    typedef Symbol<Type<Simple> > InterfaceBaseType;

     class Interface : public InterfaceBaseType,
                       public boost::enable_shared_from_this<Symbol<Type<Integral> > > {
     private:
       typedef InterfaceBaseType BaseType;
       std::uint64_t typeSize;

     public:
       Interface(std::uint64_t size);

       ptr<Node<Base>>::type getSharedHandle(void) {
         return fast_cast<Node<Base>>(shared_from_this());
       };
       ptr<Node<Base>>::const_type getSharedHandle(void) const {
         return fast_cast<const Node<Base>>(shared_from_this());
       };
     };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<Simple> > VisitorBaseType;

     static std::string getName(std::uint64_t size) {
       return "int" + boost::lexical_cast<std::string>(size);
     }
     static std::string getName(std::uint64_t size, ptr<Symbol<Module> >::type) {
       return getName(size);
     }
  };
}

#endif
