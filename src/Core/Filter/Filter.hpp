#ifndef mirv_Core_Filter_Filter_hpp
#define mirv_Core_Filter_Filter_hpp

#include <mirv/Core/IR/NodeFwd.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Containers/Vector.hpp>

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

    class Dependencies {
    private:
      DependenceVector::const_iterator first;
      DependenceVector::const_iterator last;
    public:
      Dependencies(DependenceVector::const_iterator begin,
                   DependenceVector::const_iterator end)
          : first(begin), last(end) {}

      typedef DependenceVector::const_iterator iterator;
      typedef DependenceVector::const_iterator const_iterator;

      const_iterator begin(void) const {
        return first;
      }
      const_iterator end(void) const {
        return last;
      }
    };

    virtual void run(ptr<Node<Base> > node) = 0;
    virtual void run(ptr<const Node<Base> > node) = 0;

  public:
    template<typename InputIterator>
    FilterBase(InputIterator requiresBegin, InputIterator requiresEnd,
               InputIterator providesBegin, InputIterator providesEnd,
               InputIterator killsBegin, InputIterator killsEnd)
        : requiresVector(requiresBegin, requiresEnd),
            providesVector(providesBegin, providesEnd),
            killsVector(killsBegin, killsEnd) {}
    virtual ~FilterBase(void);

    Dependencies requires(void) const {
      return Dependencies(requiresVector.begin(), requiresVector.end());
    }
    Dependencies provides(void) const {
      return Dependencies(providesVector.begin(), providesVector.end());
    }
    Dependencies kills(void) const {
      return Dependencies(killsVector.begin(), killsVector.end());
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
    template<typename InputIterator>
    Filter(InputIterator requiresBegin, InputIterator requiresEnd,
           InputIterator providesBegin, InputIterator providesEnd,
           InputIterator killsBegin, InputIterator killsEnd)
        : FilterBase(requiresBegin, requiresEnd,
                     providesBegin, providesEnd,
                     killsBegin, killsEnd) {}

    typedef Result result_type;
  };

  class NullDependence {
  public:
    typedef const std::vector<std::string> StringVector;

    static StringVector::const_iterator begin() {
      return nullVector.begin();
    }
    static StringVector::const_iterator end() {
      return nullVector.end();
    }

  private:
    static StringVector nullVector;
  };
}

#endif
