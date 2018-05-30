#ifndef mirv_Core_IR_GlobalVariable_hpp
#define mirv_Core_IR_GlobalVariable_hpp

#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Filter/ValueVisitor.hpp>
#include <mirv/Core/IR/Global.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Library/TypeList.hpp>

namespace mirv {
  class GlobalVariable : public Global,
                         public Symbol {
    friend class Module;
    friend class IRBuilder;

    typedef ptr<Constant> ConstantPtr;
    ConstantPtr Initializer;

    ptr<Module> Parent;

    GlobalVariable(const std::string Name, ptr<const Type> T,
                   ConstantPtr Init = ConstantPtr()) :
      Global(std::move(Name), T), Initializer(Init) {}

    void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    void acceptImpl(SymbolVisitor &V) override {
      V.visit(*this);
    }

    void setParent(ptr<Module> P) {
      Parent = P;
    }

    static ptr<GlobalVariable>
    Make(const std::string Name, ptr<const Type> T,
         ptr<Constant> Init = nullptr) {
      return getHandle(new GlobalVariable(std::move(Name), T, Init));
    }

  public:
    using Global::accept;
    using Symbol::accept;

    typedef TypeList<Global, Symbol> VisitorBaseTypes;

    ConstantPtr getInitializer(void) const {
      return Initializer;
    }
  };
}

#endif
