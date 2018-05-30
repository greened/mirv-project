#ifndef mirv_Core_IR_ValueConsumer_hpp
#define mirv_Core_IR_ValueConsumer_hpp

#include <mirv/Core/IR/Consumer.hpp>
#include <mirv/Core/IR/Dependence.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Library/List.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/TypeList.hpp>
#include <mirv/Library/Vector.hpp>

#include <utility>

namespace mirv {
  class Control;
  class ValueProducer;

  class ValueConsumer : public Consumer {
    typedef List<ValueDependence> DependenceList;

    DependenceList TheDependencies;

    // TODO: True data dependence.

    ValueConsumer(void) = delete;
    ValueConsumer(const ValueConsumer &) = delete;
    ValueConsumer(const ValueConsumer &&) = delete;
    ValueConsumer &operator=(const ValueConsumer &) = delete;
    ValueConsumer &operator=(const ValueConsumer &&) = delete;

  private:
    ValueConsumer(unsigned I, ptr<ValueProducer> P) {
      auto ThisHandle = getHandle(this);
      TheDependencies.emplace_back(P, ThisHandle, I);
    }

    template<typename ...Args>
    ValueConsumer(unsigned I, ptr<ValueProducer> P, Args ...args) :
      ValueConsumer(I + 1, fast_cast<ValueProducer>(args)...) {
      auto ThisHandle = getHandle(this);
      TheDependencies.emplace_back(P, ThisHandle, I);
    }

  protected:
    // Single entry point to contructing consumers.  Initialize the
    // operands index.
    template<typename ...Args>
    ValueConsumer(Args ...args) :
      ValueConsumer(0U, fast_cast<ValueProducer>(args)...) {
      // The vector is now in reverse order.  Fix that.  An
      // alternative would be to return reverse iterators in the
      // ranges but examining data in a debugger would be confusing.
      std::reverse(TheDependencies.begin(), TheDependencies.end());
    }

    template<typename Iterator>
    ValueConsumer(Range<Iterator> R) {
      unsigned I = 0;
      for (auto V : R) {
        TheDependencies.emplace_back(V, getHandle(this), I++);
      }
    }

    typedef ValueDependence::ProducerHandle ProducerHandle;

    typedef DependenceList::iterator operand_iterator;
    typedef DependenceList::reverse_iterator reverse_operand_iterator;
    typedef DependenceList::const_iterator const_operand_iterator;
    typedef DependenceList::const_reverse_iterator const_reverse_operand_iterator;

    typedef DependenceList::size_type operand_size_type;

    typedef Range<operand_iterator> DependenceRange;
    typedef Range<const_operand_iterator> ConstDependenceRange;

  public:
    using VisitorBaseTypes = TypeList< Consumer>;

    auto Dependencies(void) {
      return DependenceRange(TheDependencies.begin(), TheDependencies.end());
    }

    auto Dependencies(void) const {
      return ConstDependenceRange(TheDependencies.begin(),
                                  TheDependencies.end());
    }

    auto Operands(void) {
      return TransformRange(Dependencies(), [](const ValueDependence &D) {
          return D.getProducer();
        });
    }

    auto Operands(void) const {
      return TransformRange(Dependencies(), [](const ValueDependence &D) {
          return D.getProducer();
        });
    }

    bool OperandsEmpty(void) const {
      return TheDependencies.empty();
    }

    std::size_t OperandsSize(void) const {
      return TheDependencies.size();
    }
  };
};

#endif
