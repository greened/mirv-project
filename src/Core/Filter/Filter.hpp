#ifndef mirv_Core_Filter_Filter_hpp
#define mirv_Core_Filter_Filter_hpp

#include <mirv/Core/IR/NodeFwd.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Containers/Vector.hpp>

#include <boost/range/iterator_range.hpp>

namespace mirv {
  /// This is the base class for all IR filters.  A filter is
  /// essenetially a pass that examines and/or modifies the IR in some
  /// way.
  class FilterBase {
  private:
    typedef Vector<std::string>::type DependenceVector;
    DependenceVector requiresVector;
    DependenceVector providesVector;
    DependenceVector killsVector;

    virtual void run(ptr<Node<Base> > node) = 0;
    virtual void run(ptr<const Node<Base> > node) = 0;

  public:
    template<typename ForwardRange>
    FilterBase(ForwardRange require,
               ForwardRange provide,
               ForwardRange kill)
        : requiresVector(require.begin(), require.end()),
            providesVector(provide.begin(), provide.end()),
            killsVector(kill.begin(), kill.end()) {}
    virtual ~FilterBase(void);

    typedef boost::iterator_range<DependenceVector::const_iterator> range;

    range requires(void) const {
      return range(requiresVector.begin(), requiresVector.end());
    }
    range provides(void) const {
      return range(providesVector.begin(), providesVector.end());
    }
    range kills(void) const {
      return range(killsVector.begin(), killsVector.end());
    }

    void operator()(ptr<Node<Base> > node) {
      run(node);
    }
    void operator()(ptr<const Node<Base> > node) {
      run(node);
    }
  };

  template<typename Visited, typename Result = void>
  class Filter : public FilterBase {
  private:
    virtual void run(ptr<Node<Base> > node) {}
    virtual void run(ptr<const Node<Base> > node) {}

  public:
    template<typename ForwardRange>
    Filter(ForwardRange requires,
           ForwardRange provides,
           ForwardRange kills)
        : FilterBase(requires, provides, kills) {}

    typedef Result result_type;
  };
}

#endif
