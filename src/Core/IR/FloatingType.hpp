#ifndef mirv_Core_IR_FloatingType_hpp
#define mirv_Core_IR_FloatingType_hpp

#include <mirv/Core/IR/Type.hpp>

#include <boost/lexical_cast.hpp>

namespace mirv {
  /// Floating point types.
   struct Floating {
    typedef Symbol<Type<Simple> > InterfaceBaseType;

     class Interface : public InterfaceBaseType {
     public:
       Interface(BitSizeType size) :
           InterfaceBaseType("float" + boost::lexical_cast<std::string>(size),
                             size) {}
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
