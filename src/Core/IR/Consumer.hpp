#ifndef mirv_Core_IR_Consumer_hpp
#define mirv_Core_IR_Consumer_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Library/TypeList.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/Vector.hpp>

#include <initializer_list>

namespace mirv {
  class Control;
  class Producer;
  class ValueVisitor;

  class Consumer {
    Consumer(const Consumer &) = delete;
    Consumer(const Consumer &&) = delete;
    Consumer &operator=(const Consumer &) = delete;
    Consumer &operator=(const Consumer &&) = delete;

    virtual void acceptImpl(ValueVisitor &);

  protected:
    Consumer(void) = default;
    virtual ~Consumer(void) = default;

  public:
    using VisitorBaseTypes = TypeList<>;

    void accept(ValueVisitor &V) {
      acceptImpl(V);
    }
  };
};

#endif
