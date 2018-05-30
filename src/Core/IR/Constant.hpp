#ifndef mirv_Core_IR_Constant_hpp
#define mirv_Core_IR_Constant_hpp

#include <mirv/Core/IR/NodeName.hpp>
#include <mirv/Core/IR/NodeType.hpp>
#include <mirv/Core/IR/ValueProducer.hpp>
#include <mirv/Library/TypeList.hpp>

namespace mirv {
  class Constant : public ValueProducer {
  private:
    NodeName TheName;
    NodeType TheType;

    ptr<const Type> getTypeImpl(void) const override {
      return TheType.getType();
    }

    std::string getNameImpl(void) const override {
      return TheName.getName();
    }

  protected:
    Constant(const std::string N, ptr<const Type> T) :
      ValueProducer(), TheName(std::move(N)), TheType(T) {}

  public:
    typedef TypeList<ValueProducer> VisitorBaseTypes;
  };
}

#endif
