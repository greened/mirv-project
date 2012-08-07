#ifndef mirv_Core_IR_Node_hpp
#define mirv_Core_IR_Node_hpp

#include <Core/Containers/Vector.hpp>
#include <Core/IR/Index.hpp>

#include <boost/range/iterator_range.hpp>

namespace mirv {
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
