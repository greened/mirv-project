#ifndef mirv_Core_IR_detail_IRNode_hpp
#define mirv_Core_IR_detail_IRNode_hpp

#include <Core/IR/Index.hpp>
#include <Core/Utils/Debug.hpp>

namespace mirv {
  class Module;
  class Function;
  class Variable;
  class Statement;
  class Expression;

  namespace detail {
    // This represents a general reference to some IR node.  It includes
    // a type tag so we know which table to index.
    class IRNode {
    public:
      enum Kind {
        Module,
        Function,
        Variable,
        Expression.
        Statement
      };

      Kind kind(void) const {
        return theKind;
      }

      template<typename T>
      Index<T> getIndex(void) const {
        checkInvariant(kind() == T::IRKind, "Table index mismatch");
        return Index<T>(index);
      }

    private:
      Kind theKind;

      typedef long long IndexType;
      IndexType index;
    };
  }
}
#endif
