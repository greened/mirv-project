#ifndef mirv_Core_IR_Global_hpp
#define mirv_Core_IR_Global_hpp

#include <mirv/Core/IR/Constant.hpp>

namespace mirv {
  class ValueVisitor;

  class Global : public Constant {
    ptr<const Type> ValType;

  protected:
    Global(const std::string N, ptr<const Type> T);

  public:
    using VisitorBaseTypes = TypeList<Constant>;

    ptr<const Type> ValueType(void) const {
      return ValType;
    }
  };
}

#endif
