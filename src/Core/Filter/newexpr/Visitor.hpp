#ifndef mirv_Core_Filter_Visitor_hpp
#define mirv_Core_Filter_Visitor_hpp

#include <mirv/Core/IR/newexpr/Node.hpp>

namespace mirv {
  /// This is the default implementation for all visitors.
  template<typename T>
  class Visitor {
  public:
    template<T::Kind K>
    void visit(T Node) { }
  };

  /// This is the visitor dispatch routine.
  template<typename V, typename K>
  class VisitorDispatch {};

  /// This is the actual visitor dispatch implementation.
  template<typename V, typename ...K>
  class VisitorDispatch<V, KindTuple<K...>> {
  private:
    template<typename N, typename Kind>
      static void dispatch(V visitor, N node) {
      visitor.visit<Kind>(N);
    }

  public:
    template<typename N>
      void operator()(V visitor, N node) {
      using F = void(V, N);
      F *table[] = {
        dispatch<V, K>
        ...
      }
      table[node.kind()](V, N);
    }
  };
}

#endif
