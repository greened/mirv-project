#ifndef mirv_Core_Builder_PointerTypeGrammar_hpp
#define mirv_Core_Builder_PointerTypeGrammar_hpp

#include <mirv/Core/Builder/PointerTypeRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/IR/PointerTypeFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for pointer types.
    struct PointerTypeBuilder : boost::proto::when<
      PointerTypeRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        UnaryConstructSymbol<Symbol<Type<Pointer> >, ModuleScope>(
          boost::proto::_data,
          TypeBuilder(boost::proto::_child)))
      > {};

    /// This is the grammar for looking up pointer types.  It differs
    /// from the construct grammar in that it assumes the base type
    /// already exists.  This avoids the problem of creating and
    /// returning placeholders for struct types.
    struct PointerTypeLookupBuilder : boost::proto::when<
      PointerTypeRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        UnaryConstructSymbol<Symbol<Type<Pointer> >, ModuleScope>(
          boost::proto::_data,
          TypeLookupBuilder(boost::proto::_child)))
      > {};
  }
}

#endif
