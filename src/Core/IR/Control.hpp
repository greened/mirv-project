#ifndef mirv_Core_IR_Control_hpp
#define mirv_Core_IR_Control_hpp

#include <mirv/Core/IR/Attributable.hpp>
#include <mirv/Core/IR/NodeName.hpp>
#include <mirv/Core/IR/ValueConsumer.hpp>
#include <mirv/Core/IR/ValueProducer.hpp>
#include <mirv/Core/Filter/ControlVisitor.hpp>
#include <mirv/Core/Filter/ValueVisitor.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Library/List.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/TypeList.hpp>
#include <mirv/Library/Vector.hpp>

namespace mirv {
  class Block;
  class ControlVisitor;
  class ConstControlVisitor;
  class Function;
  class Module;
  class Relational;

  class Control : public Attributable {
    friend class Function;

    NodeName TheName;

    ptr<Function> ParentFunction;
    ptr<Control> Parent;

  protected:
    Control(std::string Name) : TheName(std::move(Name)) {}

    void setFunctionParent(ptr<Function> F) {
      ParentFunction = F;
    }

    virtual void acceptImpl(ControlVisitor &);

  public:
    using VisitorBaseTypes = TypeList<>;

    virtual ~Control(void) = default;

    std::string getName(void) const {
      return TheName.getName();
    }

    ptr<Module> getParentModule(void) const;

    ptr<Function> getParentFunction(void) const;

    template<typename T>
    ptr<T> getControlParent(void) const {
      if (Parent) {
        auto P = dyn_cast<T>(Parent);
        if (P) {
          return P;
        }
        return Parent->getControlParent<T>();
      }
      return nullptr;
    }

    void setControlParent(ptr<Control> P) {
      Parent = P;
    }

    void accept(ControlVisitor &V) {
      acceptImpl(V);
    }

    void dump(void);
  };

  class ValueControl : public Control,
                       public ValueConsumer {
    virtual void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }
    virtual void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

  protected:
    ValueControl(std::string Name, ptr<ValueProducer> P) :
      Control(std::move(Name)), ValueConsumer{P} {}

  public:
    using VisitorBaseTypes = TypeList<Control>;

    using Control::accept;
    using ValueConsumer::accept;
  };
}


#endif
