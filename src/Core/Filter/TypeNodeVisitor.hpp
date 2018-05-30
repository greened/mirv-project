#ifndef mirv_Core_Filter_SymbolNodeVisitor_hpp
#define mirv_Core_Filter_SymbolNodeVisitor_hpp

#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Filter/ConstSymbolVisitor.hpp>

namespace mirv {
  namespace detail {
    /// This is a specialization for types because they can only ever
    /// accept ConstSymbolVisitors.  This specialization avoids the
    /// need to include type tag definitions all over and also catches
    /// visitor mismatches at compile time.
    class AcceptImpl<Type, true> {
    public:
      // FIXME: These shouldn't be needed but we get instantiation
      // errors with seemingly unrelated visitors.  Check later with a
      // compiler that has better error reporting.
      void operator()(Type *This, SymbolVisitor &V) {
        error("A type cannot accept a non-const visitor");
      }
      void operator()(const Type *This, SymbolVisitor &V) {
        error("A type cannot accept a non-const visitor");
      }

      void operator()(const Type *This, ConstSymbolVisitor &V) {
        ptr<const Type> p(This->shared_from_this());
        V.visit(p);
      }
    };

    /// Implement the accept method for type symbols.  Specialze for
    /// types, which must be const.
    class AcceptImpl<Type, false> {
    public:
      // FIXME: These shouldn't be needed but we get instantiation
      // errors with seemingly unrelated visitors.  Check later with a
      // compiler that has better error reporting.
      void operator()(Type *This, SymbolVisitor &V) {
        error("A type cannot accept a non-const visitor");
      }
      void operator()(const Type *This, SymbolVisitor &V) {
        error("A type cannot accept a non-const visitor");
      }

      void operator()(const Type *This, ConstSymbolVisitor &V) {
        error("Wrong accept called");
      }
    };
  }
}

#endif
