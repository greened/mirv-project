#ifndef mirv_Core_IR_Node_hpp
#define mirv_Core_IR_Node_hpp

#include <Core/Containers/Vector.hpp>
#include <Core/IR/Index.hpp>

#include <boost/range/iterator_range.hpp>

namespace mirv {
  /// This is a collection of node kinds.  We use it to categorize
  /// various kinds of nodes.  In particular, the visitor patterns use
  /// this to dispatch to the correct visitation routine.
  template<typename T, T::Kind ...K>
  class KindTuple {};

  template<typename T>
  class KindTuple<T> {};

  /// This is the common interface for all IR node types.
  template<typename T>
  class Node {
  private:
    typename T::Kind kind;

    typedef Vector<Index<T>> IndexVector;
    IndexVector children;

  public:
    typedef boost::iterator_range<IndexVector::iterator> range;
    typedef boost::iterator_range<IndexVector::const_iterator> const_range;
  };
}

#endif
