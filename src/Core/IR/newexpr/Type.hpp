#ifndef mirv_Core_IR_Type_hpp
#define mirv_Core_IR_Type_hpp

#include <Core/Containers/Vector.hpp>
#include <Core/IR/Index.hpp>
#include <Core/IR/Node.hpp>

#include <boost/range/iterator_range.hpp>

namespace mirv {
  class Type : public Node<Type> {
  public:
    enum Kind {
      Null,
      Integer,
      Floating,
    };
  };
}

#endif
