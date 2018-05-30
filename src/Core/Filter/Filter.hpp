#ifndef mirv_Core_Filter_Filter_hpp
#define mirv_Core_Filter_Filter_hpp

#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Library/Range.hpp>
#include <mirv/Library/Vector.hpp>

#include <boost/range/iterator_range.hpp>

namespace mirv {
  class Control;
  class Function;
  class Module;
  class Producer;

  /// This is the base class for all IR filters.  A filter is
  /// essenetially a pass that examines and/or modifies the IR in some
  /// way.
  class FilterBase {
  private:
    typedef Vector<std::string> DependenceVector;
    DependenceVector requiresVector;
    DependenceVector providesVector;
    DependenceVector killsVector;

  public:
    template<typename ForwardRange>
    FilterBase(ForwardRange require,
               ForwardRange provide,
               ForwardRange kill)
      : requiresVector(require.begin(), require.end()),
        providesVector(provide.begin(), provide.end()),
        killsVector(kill.begin(), kill.end()) {}
    virtual ~FilterBase(void);

    typedef Range<DependenceVector::const_iterator> range;

    range requires(void) const {
      return range(requiresVector.begin(), requiresVector.end());
    }
    range provides(void) const {
      return range(providesVector.begin(), providesVector.end());
    }
    range kills(void) const {
      return range(killsVector.begin(), killsVector.end());
    }
  };

  class Filter : public FilterBase {
  private:
    virtual void runImpl(ptr<Module> M);
    virtual void runImpl(ptr<Function> F) = 0;
    // virtual void runImpl(ptr<Symbol> F) = 0;
    virtual void runImpl(ptr<Producer> F) = 0;
    virtual void runImpl(ptr<Control> F) = 0;

  public:
    template<typename ForwardRange>
    Filter(ForwardRange requires,
           ForwardRange provides,
           ForwardRange kills) :
      FilterBase(requires, provides, kills) {}

    void run(ptr<Module> M) {
      runImpl(M);
    }
    void run(ptr<Function> F) {
      runImpl(F);
    }
    // void run(ptr<Symbol> F) {
    //   runImpl(F);
    // }
    void run(ptr<Producer> F) {
      runImpl(F);
    }
    void run(ptr<Control> F) {
      runImpl(F);
    }
  };
}

#endif
