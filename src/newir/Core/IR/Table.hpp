#ifndef mirv_Core_IR_Table_hpp
#define mirv_Core_IR_Table_hpp

#include <Core/Containers/Vector.hpp>
#include <mirv/Core/IR/Index.hpp>

#include <boost/range/iterator_range.hpp>

namespace mirv {
  /// A generic table.
  template<typename T>
  class Table {
  public:
    typedef T ValueType;

  private:
    typedef Vector<ValueType> VectorType;
    VectorType data;

  public:
    typedef boost::iterator_range<VectorType::iterator> range;
    typedef boost::iterator_range<VectorType::const_iterator> const_range;

    range values(void) {
      return boost::make_iterator_range(data.begin(), data.end());
    }
    range values(void) const {
      return boost::make_iterator_range(data.begin(), data.end());
    }

    ValueType &operator[](Index<T> i) {
      checkInvariant(i.get() < data.size(), "Table index out of bounds");
      return data[i.get()];
    }

    const ValueType operator[](Index<T> i) {
      checkInvariant(i.get() < data.size(), "Table index out of bounds");
      return data[i.get()];
    }
  };
}

#endif
