#ifndef mirv_Core_Filter_Visitor_hpp
#define mirv_Core_Filter_Visitor_hpp

#include <mirv/Core/IR/newexpr/Node.hpp>

namespace mirv {
  /// This is the default implementation for all visitors.
  template<typename V>
  class Visitor {
  private:
    template<typename T>
    class VisitFallback {};

    template<V::VisitKind::K, V::VisitKind::Kind ...Rest>
    class VisitFallback<KindTuple<K, Rest...>> {
    public:
      template<typename N>
      static void dispatch(V visitor, N node) {
        visitor.visit<K>(N);
        VisitFallback<KindTuple<Rest...>>::dispatch(visitor, node);
      }
    };

  public:
    template<V::VisitKind::Kind K>
      void visit(V visitor, T Node) {
      // Visit using the default fallback visit routines.
      VisitFallback<typename VisitKind::VisitKinds<K>::type>::
        dispatch(visitor, node);
   }
  };

  template<typename V>
  template<V::VisitKind K>
  class Visitor<V>::VisitFallback<KindTuple<K>> {
  public:
    template<typename N>
    static void dispatch(V visitor, N node) {
      visitor.visit<K>(N);
    }
  };

  template<typename V>
  template<>
  class Visitor<V>::VisitFallback<KindTuple<>> {
  public:
    template<typename N>
    static void dispatch(V visitor, N node) {}
  };

  /// This is the visitor dispatch mechanism.  It statically generates
  /// a lookup table of visit routines, one for each node Kind.
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
