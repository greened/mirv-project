#ifndef mirv_Core_IR_Node_hpp
#define mirv_Core_IR_Node_hpp

#include <Core/Containers/Vector.hpp>

#include <boost/range/iterator_range.hpp>

namespace mirv {
  /// This is the common interface for all IR node types.
  template<typename T>
  class Node {
  private:

  public:
    typedef boost::iterator_range<IndexVector::iterator> Range;
    typedef boost::iterator_range<IndexVector::const_iterator> ConstRange;

    template<typename U = T>
    Range children(void) {
      return Range(theChildren.begin(), theChildren.end());
    }

    template<typename U = T>
    ConstRange children(void) const {
      return Range(theChildren.begin(), theChildren.end());
    }

  private:
    typedef Vector<long int> IndexVector;
    IndexVector theChildren;
  };
}

#endif
