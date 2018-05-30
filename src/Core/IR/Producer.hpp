#ifndef mirv_Core_IR_Producer_hpp
#define mirv_Core_IR_Producer_hpp

#include <mirv/Core/IR/Attributable.hpp>
#include <mirv/Core/IR/Dependence.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Library/List.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/Vector.hpp>

#include <string>

namespace mirv {
  class Control;
  class Function;
  class Module;
  class ValueVisitor;
  class Sequence;
  class Type;
  class ValueDependence;

  class Producer : public Attributable {
    friend class Sequence;

    ptr<Sequence> Parent;

    virtual void acceptImpl(ValueVisitor &);

    void setSequenceParent(ptr<Sequence> S) {
      Parent = S;
    }

    ptr<Control> getControlParentHelper(void) const;

    Producer(const Producer &) = delete;
    Producer(const Producer &&) = delete;
    Producer &operator=(const Producer &) = delete;
    Producer &operator=(const Producer &&) = delete;

  public:
    typedef ptr<const Type> TypeHandle;

  private:
    virtual std::string getNameImpl(void) const = 0;
    virtual TypeHandle getTypeImpl(void) const = 0;

  protected:
    Producer(void) = default;
    virtual ~Producer(void) = default;

  public:
    using VisitorBaseTypes = TypeList<>;

    TypeHandle getType(void) const {
      return getTypeImpl();
    }

    std::string getName(void) const {
      return getNameImpl();
    }

    void accept(ValueVisitor &V) {
      acceptImpl(V);
    }

    ptr<Module> getParentModule(void) const;

    ptr<Function> getParentFunction(void) const;

    ptr<Sequence> getSequenceParent(void) const {
      return Parent;
    }

    ptr<Control> getControlParent(void) const {
      ptr<Control> C = getControlParentHelper();
      return C;
    }

    void dump(void);
  };
};

#endif
