#ifndef mirv_Core_Filter_SymbolNodeVisitor_hpp
#define mirv_Core_Filter_SymbolNodeVisitor_hpp

#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>
#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Filter/ConstSymbolVisitor.hpp>

namespace mirv {
  namespace detail {
    /// This is a specialization for types because they can only ever
    /// accept ConstSymbolVisitors.  This specialization avoids the
    /// need to include type tag definitions all over and also catches
    /// visitor mismatches at compile time.
    template<typename Tag>
    class AcceptImpl<Symbol<Type<Tag> >, true> {
    public:
      // FIXME: These shouldn't be needed but we get instantiation
      // errors with seemingly unrelated visitors.  Check later with a
      // compiler that has better error reporting.
      void operator()(Symbol<Type<Tag> > *This, SymbolVisitor &V) {
        error("A type cannot accept a non-const visitor");
      }
      void operator()(const Symbol<Type<Tag> > *This, SymbolVisitor &V) {
        error("A type cannot accept a non-const visitor");
      }

      void operator()(const Symbol<Type<Tag> > *This, ConstSymbolVisitor &V) {
        typename ptr<Symbol<Type<Tag> > >::const_type p(This->shared_from_this());
        V.visit(p);
      }
    };

    template<typename Tag>
    class AcceptImpl<Symbol<Type<Tag> >, false> {
    public:
      // FIXME: These shouldn't be needed but we get instantiation
      // errors with seemingly unrelated visitors.  Check later with a
      // compiler that has better error reporting.
      void operator()(Symbol<Type<Tag> > *This, SymbolVisitor &V) {
        error("A type cannot accept a non-const visitor");
      }
      void operator()(const Symbol<Type<Tag> > *This, SymbolVisitor &V) {
        error("A type cannot accept a non-const visitor");
      }

      void operator()(const Symbol<Type<Tag> > *This, ConstSymbolVisitor &V) {
        error("Wrong accept called");
      }
    };
  }
}

#endif
