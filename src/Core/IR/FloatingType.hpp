#ifndef mirv_Core_IR_FloatingType_hpp
#define mirv_Core_IR_FloatingType_hpp

#include <mirv/Core/IR/Type.hpp>

#include <boost/lexical_cast.hpp>

namespace mirv {
  /// Floating point types.
   struct Floating {
    typedef Symbol<Type<Simple> > InterfaceBaseType;

     class Interface : public InterfaceBaseType,
                       public boost::enable_shared_from_this<Symbol<Type<Floating> > > {
     public:
       Interface(BitSizeType size) :
           InterfaceBaseType("float" + boost::lexical_cast<std::string>(size),
                             size) {}

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

     static std::string getName(Interface::BitSizeType size) {
       return "float" + boost::lexical_cast<std::string>(size);
     }
  };
}

#endif
