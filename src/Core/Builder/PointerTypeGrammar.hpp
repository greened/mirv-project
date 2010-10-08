#ifndef mirv_Core_Builder_PointerTypeGrammar_hpp
#define mirv_Core_Builder_PointerTypeGrammar_hpp

#include <mirv/Core/Builder/PointerTypeRules.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/IR/PointerType.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for pointer types.
    struct PointerTypeBuilder : boost::proto::when<
      PointerTypeRule,
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >(
        boost::proto::_data,
        UnaryConstructSymbol<Symbol<Type<Pointer> > >(
          boost::proto::_data,
          TypeBuilder(boost::proto::_child)))
      > {};
  }
}

#endif