#ifndef mirv_Core_IR_DataProducer_hpp
#define mirv_Core_IR_DataProducer_hpp

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
  class Sequence;
  class Type;
  class DataDependence;

  class DataProducer : public Producer {
    typedef IntrusiveList<DataDependence, DataProducer> DataUseList;
    DataUseList TheDataUses;

    void acceptImpl(ValueVisitor &V) override;

    friend class DataDependence;

    void addDependence(DataDependence &D);
    void removeDependence(DataDependence &D);

    typedef DataUseList::iterator value_iterator;
    typedef DataUseList::const_iterator const_value_iterator;

    DataProducer(const DataProducer &) = delete;
    DataProducer(const DataProducer &&) = delete;
    DataProducer &operator=(const DataProducer &) = delete;
    DataProducer &operator=(const DataProducer &&) = delete;

  protected:
    DataProducer(void) = default;
    virtual ~DataProducer(void) = default;

  public:
    using VisitorBaseTypes = TypeList<Producer>;

    typedef Range<value_iterator> DataUseRange;
    typedef Range<const_value_iterator> ConstDataUseRange;

    DataUseRange DataUses(void) {
      return DataUseRange(TheDataUses.begin(), TheDataUses.end());
    }
    ConstDataUseRange DataUses(void) const {
      return ConstDataUseRange(TheDataUses.begin(), TheDataUses.end());
    }

    auto NumDataUses(void) const {
      return TheDataUses.size();
    }
  };
};

#endif
