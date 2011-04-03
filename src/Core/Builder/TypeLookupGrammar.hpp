#ifndef mirv_Core_Builder_TypeLookupGrammar_hpp
#define mirv_Core_Builder_TypeLookupGrammar_hpp

#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/SymbolTerminals.hpp>
#include <mirv/Core/Builder/TypeGrammar.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is the grammar to look up a type.  In the case of
    /// constructed types, see if it already exists in the symbol table
    /// and add it if not.
    struct TypeAccessBuilder : boost::proto::or_<
      boost::proto::when<
        StringTerminal,
        LookupSymbol<Symbol<Type<TypeBase> > >(
          boost::proto::_data,
          boost::proto::_value)
        >,
      TypeLookupBuilder
      > {};

    struct FunctionTypeAccessBuilder : boost::proto::or_<
      boost::proto::when<
        StringTerminal,
        LookupSymbol<Symbol<Type<TypeBase> > >(
          boost::proto::_data,
          boost::proto::_value)
        >,
      FunctionTypeBuilder
      > {};
  }
}

#endif
