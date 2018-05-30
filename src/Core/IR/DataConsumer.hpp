#ifndef mirv_Core_IR_DataConsumer_hpp
#define mirv_Core_IR_DataConsumer_hpp

#include <mirv/Core/IR/Consumer.hpp>
#include <mirv/Core/IR/Dependence.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/TypeList.hpp>
#include <mirv/Library/Vector.hpp>

#include <initializer_list>

namespace mirv {
  class Control;
  class DataProducer;

  class DataConsumer : public Consumer {
    typedef DataDependence &DependenceHandle;
    typedef const DataDependence &ConstDependenceHandle;
    typedef Vector<DataDependence> DependenceList;

    DependenceList Depedencies;

    // TODO: True data dependence.

    DataConsumer(const DataConsumer &) = delete;
    DataConsumer(const DataConsumer &&) = delete;
    DataConsumer &operator=(const DataConsumer &) = delete;
    DataConsumer &operator=(const DataConsumer &&) = delete;

  protected:
    using ProducerHandle = ptr<DataProducer>;

    DataConsumer(void) = default;
    DataConsumer(ProducerHandle P) {
      //Dependences.emplace_back(P, getHandle(this), 0);
      // auto V = DataDependence(P, getHandle(this), 0);
      // Depedencies.push_back(V);
    }

    template<typename Range>
    DataConsumer(const Range &R) {
      // unsigned int i = 0;
      // for (auto P : R) {
        //Dependences.emplace_back(P, getHandle(this), i++);
        // auto V = DataDependence(P, getHandle(this), i++);
        // Depedencies.push_back(V);
      // }
    }

    typedef DependenceList::iterator operand_iterator;
    typedef DependenceList::reverse_iterator reverse_operand_iterator;
    typedef DependenceList::const_iterator const_operand_iterator;
    typedef DependenceList::const_reverse_iterator const_reverse_operand_iterator;

    typedef DependenceList::size_type operand_size_type;

  public:
    using VisitorBaseTypes = TypeList<Consumer>;

    // typedef Range<operand_iterator> DependenceRange;
    // typedef Range<const_operand_iterator> ConstDependenceRange;

    // DependenceRange Dependencies(void) {
    //   return DependenceRange(Depedencies.begin(), Depedencies.end());
    // }
    // ConstDependenceRange Dependencies(void) const {
    //   return ConstDependenceRange(Depedencies.begin(), Depedencies.end());
    // }

    // auto Operands(void) {
    //   return TransformRange(Dependencies(), [](const DataDependence &D) {
    //       return D.getProducer();
    //     });
    // }

    // auto Operands(void) const {
    //   return TransformRange(Dependencies(), [](const DataDependence &D) {
    //       return D.getProducer();
    //     });
    // }
  };
};

#endif
