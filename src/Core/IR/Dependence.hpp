#ifndef mirv_Core_IR_Dependence_hpp
#define mirv_Core_IR_Dependence_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Library/List.hpp>

namespace mirv {
  class ValueProducer;
  class ValueConsumer;
  class DataProducer;
  class DataConsumer;

  class Dependence {
  };

  // This represents a true dependence between Producer and Consumer.
  // Consumer owns the ValueDependence objects and they are linked
  // into the use list of ValueProducer.
  class ValueDependence final : public Dependence,
                                public IntrusiveListBaseHook<ValueProducer> {
  public:
    typedef ptr<ValueProducer> ProducerHandle;
    typedef ptr<ValueConsumer> ConsumerHandle;
    typedef std::size_t ConsumerOperandIndex;

  private:
    ProducerHandle TheProducer;
    ConsumerHandle TheConsumer;
    ConsumerOperandIndex OperandIndex;

  public:
    ValueDependence(ProducerHandle P, ConsumerHandle C, ConsumerOperandIndex O);

    ~ValueDependence(void);

    ProducerHandle getProducer(void) const {
      return TheProducer;
    }

    ConsumerHandle getConsumer(void) const {
      return TheConsumer;
    }

    ConsumerOperandIndex getOperandIndex(void) const {
      return OperandIndex;
    }

    void swap(ValueDependence &Other) {
      using std::swap;
      swap(TheProducer, Other.TheProducer);
      swap(TheConsumer, Other.TheConsumer);
      swap(OperandIndex, Other.OperandIndex);
    }
  };

  inline void swap(ValueDependence &A, ValueDependence &B) {
    A.swap(B);
  }

  class DataDependence : public Dependence,
                         public IntrusiveListBaseHook<DataProducer> {
  };

  class TrueDataDependence final : public DataDependence {
    typedef ptr<DataProducer> ProducerHandle;
    ProducerHandle TheProducer;

  public:
    TrueDataDependence(ProducerHandle P) : TheProducer(P) {}

    ProducerHandle getProducer(void) const {
      return TheProducer;
    }
  };

  class AntiDataDependence final : public DataDependence {
    typedef ptr<DataConsumer> ConsumerHandle;
    ConsumerHandle TheConsumer;

  public:
    AntiDataDependence(ConsumerHandle C) : TheConsumer(C) {}

    ConsumerHandle getConsumer(void) const {
      return TheConsumer;
    }
  };

  class OutputDataDependence final : public DataDependence {
    typedef ptr<DataProducer> ProducerHandle;
    ProducerHandle TheProducer;

  public:
    OutputDataDependence(ProducerHandle P) : TheProducer(P) {}

    ProducerHandle getProducer(void) const {
      return TheProducer;
    }
  };
};

#endif
