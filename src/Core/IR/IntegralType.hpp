#ifndef mirv_Core_IR_IntegralType_hpp
#define mirv_Core_IR_IntegralType_hpp

#include <mirv/Core/IR/Type.hpp>

#include <boost/lexical_cast.hpp>

namespace mirv {
  /// Integer types.
   struct Integral {
   private:
    typedef Symbol<Type<Simple> > InterfaceBaseType;

     class Interface : public InterfaceBaseType,
                       public boost::enable_shared_from_this<Symbol<Type<Integral> > > {
     public:
       Interface(BitSizeType size) :
           InterfaceBaseType("int" + boost::lexical_cast<std::string>(size),
                             size) {}
     };

   public:
     typedef Interface BaseType;
     typedef Symbol<Type<Simple> > VisitorBaseType;

     static std::string getName(Interface::BitSizeType size) {
       return "int" + boost::lexical_cast<std::string>(size);
     }
  };
}

#endif
