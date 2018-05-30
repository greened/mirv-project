#ifndef mirv_Core_IR_Module_hpp
#define mirv_Core_IR_Module_hpp

#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/NodeName.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Type.hpp>

#include <mirv/Library/Range.hpp>
#include <mirv/Library/TypeList.hpp>
#include <mirv/Library/Vector.hpp>

namespace mirv {
  class IRBuilder;

  class Module : public Symbol {
    friend class IRBuilder;
    typedef Vector<ptr<const Type>> TypeVector;
    typedef Vector<ptr<GlobalVariable>> GlobalList;
    typedef Vector<ptr<Function>> FunctionList;
    typedef Vector<ptr<Constant>> ConstantList;

    NodeName TheName;

    TypeVector AllTypes;
    GlobalList AllGlobals;
    FunctionList AllFunctions;
    ConstantList AllConstants;

    Module(const std::string Name) : TheName(std::move(Name)) {}

    void acceptImpl(SymbolVisitor &V) override {
      V.visit(*this);
    }

    void AddType(ptr<const Type> T) {
      AllTypes.push_back(T);
    }

    void AddConstant(ptr<Constant> C) {
      AllConstants.push_back(C);
    }

    void AddFunction(ptr<Function> f) {
      AllFunctions.push_back(f);
      f->setParentModule(getHandle(this));
    }

    void AddGlobalVariable(ptr<GlobalVariable> v) {
      AllGlobals.push_back(v);
    }

    static ptr<Module> Make(const std::string Name) {
      return getHandle(new Module(std::move(Name)));
    }

  public:
    using VisitorBaseTypes = TypeList<Symbol>;

    std::string getName(void) {
      return TheName.getName();
    }

    template<typename T>
    ptr<T> getControlParent(void) {
      unreachable("Could not find control parent");
      return nullptr;
    }


    ptr<Function> functionFind(const std::string &name) {
      auto i = std::find_if(AllFunctions.begin(), AllFunctions.end(),
                            [&name](ptr<Function> F) -> bool {
                              return F->getName() == name;
                            });
      if (i != AllFunctions.end()) {
        return *i;
      }
      return ptr<Function>();
    }

    ptr<GlobalVariable> globalVariableFind(const std::string &name) {
      auto i = std::find_if(AllGlobals.begin(), AllGlobals.end(),
                            [&name](ptr<GlobalVariable> G) -> bool {
                              return G->getName() == name;
                            });
      if (i != AllGlobals.end()) {
        return *i;
      }
      return ptr<GlobalVariable>();
    }

    auto Types(void) const {
      return MakeRange(AllTypes.begin(), AllTypes.end());
    }

    auto Constants(void) const {
      return MakeRange(AllConstants.begin(), AllConstants.end());
    }

    auto Globals(void) const {
      return MakeRange(AllGlobals.begin(), AllGlobals.end());
    }

    auto Functions(void) {
      return MakeRange(AllFunctions.begin(), AllFunctions.end());
    }

    auto Functions(void) const {
      return MakeRange(AllFunctions.begin(), AllFunctions.end());
    }

    void dump(void);
  };
}

#endif
