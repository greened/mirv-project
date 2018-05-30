#ifndef mirv_Core_Builder_PointerTypeGrammar_hpp
#define mirv_Core_Builder_PointerTypeGrammar_hpp

#include <mirv/Core/Builder/PointerTypeRules.hpp>
#include <mirv/Core/Builder/ConstructSymbolTransform.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Type.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar for pointer types.
    struct PointerTypeBuilder : boost::proto::when<
      PointerTypeRule,
      //LookupAndAddSymbol<Type>(
      //boost::proto::_data,
        UnaryConstructSymbol<PointerType, ModuleScope>(
          boost::proto::_data,
          TypeBuilder(boost::proto::_child))
      //)
      > {};

    /// This is the grammar for looking up pointer types.  It differs
    /// from the construct grammar in that it assumes the base type
    /// already exists.  This avoids the problem of creating and
    /// returning placeholders for struct types.
    struct PointerTypeLookupBuilder : boost::proto::when<
      PointerTypeRule,
      //LookupAndAddSymbol<Type>(
      //boost::proto::_data,
        UnaryConstructSymbol<PointerType, ModuleScope>(
          boost::proto::_data,
          TypeLookupBuilder(boost::proto::_child))
      //)
      > {};
  }
}

#endif
