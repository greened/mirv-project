#ifndef mirv_Core_IR_Sequence_hpp
#define mirv_Core_IR_Sequence_hpp

#include <mirv/Core/IR/Consumer.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Library/List.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/TypeList.hpp>

namespace mirv {
  class IRBuilder;
  class Producer;

  // A sequence node holds a top-level reference to an expression.
  // Typically, this is an expression with multiple uses and the
  // Sequence node provides it a single place to live.
  class Sequence : public Control {
    friend class IRBuilder;

    virtual void acceptImpl(ControlVisitor &V) override {
      V.visit(*this);
    }

    // We don't want this to look like a value use, so keep our own
    // handle to the Producer.
    ptr<Producer> TheProducer;

    static ptr<Sequence> Make(std::string Name, ptr<Producer> P) {
      return getHandle(new Sequence(std::move(Name), P));
    }

  protected:
    Sequence(std::string Name, ptr<Producer> P);

  public:
    using VisitorBaseTypes = TypeList<Control>;

    ptr<Producer> getProducer(void) {
      return TheProducer;
    }
    ptr<const Producer> getProducer(void) const {
      return TheProducer;
    }
  };
}

#endif
