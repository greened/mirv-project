#ifndef mirv_Core_IR_Type_hpp
#define mirv_Core_IR_Type_hpp

#include <Core/Containers/Vector.hpp>
#include <Core/IR/Index.hpp>

#include <boost/range/iterator_range.hpp>

namespace mirv {
  class Expression {
  public:
    enum Kind {
      Null,
      Integer,
      Floating,
    };


  private:
    Kind kind;

    typedef Vector<Index<Type>> IndexVector;
    IndexVector fields;

  public:
    typedef boost::iterator_range<IndexVector::iterator> range;
    typedef boost::iterator_range<VectorType::const_iterator> const_range;
  };
}

#endif
