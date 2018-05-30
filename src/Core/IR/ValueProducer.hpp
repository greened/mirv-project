#ifndef mirv_Core_IR_ValueProducer_hpp
#define mirv_Core_IR_ValueProducer_hpp

#include <mirv/Core/Filter/ValueVisitor.hpp>
#include <mirv/Core/IR/Producer.hpp>
#include <mirv/Core/IR/Dependence.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Utility/Debug.hpp>
#include <mirv/Library/List.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/TypeList.hpp>
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

  class ValueProducer : public Producer {
    typedef IntrusiveList<ValueDependence, ValueProducer> ValueUseList;
    ValueUseList TheValueUses;

    virtual void acceptImpl(ValueVisitor &V) override {
      V.visit(*this);
    }

    friend class ValueDependence;

    void addDependence(ValueDependence &D);
    void removeDependence(ValueDependence &D);

    typedef ValueUseList::iterator value_iterator;
    typedef ValueUseList::const_iterator const_value_iterator;

    ValueProducer(const ValueProducer &) = delete;
    ValueProducer(const ValueProducer &&) = delete;
    ValueProducer &operator=(const ValueProducer &) = delete;
    ValueProducer &operator=(const ValueProducer &&) = delete;

  public:
    typedef ptr<const Type> TypeHandle;

  protected:
    ValueProducer(void) = default;
    virtual ~ValueProducer(void) = default;

  public:
    using VisitorBaseTypes = TypeList<Producer>;

    typedef Range<value_iterator> ValueUseRange;
    typedef Range<const_value_iterator> ConstValueUseRange;

    ValueUseRange ValueUses(void) {
      return ValueUseRange(TheValueUses.begin(), TheValueUses.end());
    }
    ConstValueUseRange ValueUses(void) const {
      return ConstValueUseRange(TheValueUses.begin(), TheValueUses.end());
    }

    auto NumValueUses(void) const {
      return TheValueUses.size();
    }
  };
};

#endif
