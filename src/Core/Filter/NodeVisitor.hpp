#ifndef mirv_Core_Filter_NodeVisitor_hpp
#define mirv_Core_Filter_NodeVisitor_hpp

#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>
#include <mirv/Core/Filter/ConstSymbolVisitor.hpp>

namespace mirv {
  namespace detail {
    template<typename NodeType, bool HasSharedFromThis>
    class AcceptImpl {
    public:
      template<typename VisitorType>
      void operator()(NodeType *This, VisitorType &V) {
        typename ptr<NodeType>::type p(This->shared_from_this());
        V.visit(p);
      }

      template<typename VisitorType>
      void operator()(const NodeType *This, VisitorType &V) {
        typename ptr<NodeType>::const_type p(This->shared_from_this());
        V.visit(p);
      }
    };

    /// This is a specialization for types because they can only ever
    /// accept ConstSymbolVisitors.  This specialization avoids the
    /// need to include type tag definitions all over and also catches
    /// visitor mismatches at compile time.
    template<typename Tag, bool HasSharedFromThis>
    class AcceptImpl<Symbol<Type<Tag> >, HasSharedFromThis> {
    public:
      /*
      void operator()(Symbol<Type<Tag> > *This, SymbolVisitor &V) {
        error("A type cannot accept a non-const visitor");
      }
      void operator()(const Symbol<Type<Tag> > *This, SymbolVisitor &V) {
        error("A type cannot accept a non-const visitor");
      }

      void operator()(Symbol<Type<Tag> > *This, ConstSymbolVisitor &V) {
        typename ptr<Symbol<Type<Tag> > >::type p(This->shared_from_this());
        V.visit(p);
      }
      */

      void operator()(const Symbol<Type<Tag> > *This, ConstSymbolVisitor &V) {
        typename ptr<Symbol<Type<Tag> > >::const_type p(This->shared_from_this());
        V.visit(p);
      }
    };

    template<typename NodeType>
    class AcceptImpl<NodeType, false> {
    public:
      template<typename VisitorType>
      void operator()(NodeType *This, VisitorType &V) {
        error("Wrong accept called");
      }

      template<typename VisitorType>
      void operator()(const NodeType *This, VisitorType &V) {
        error("Wrong accept called");
      }
    };
  }
}

#endif
