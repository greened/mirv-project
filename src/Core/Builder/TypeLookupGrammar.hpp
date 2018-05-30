#ifndef mirv_Core_Builder_TypeLookupGrammar_hpp
#define mirv_Core_Builder_TypeLookupGrammar_hpp

#include <mirv/Core/Builder/SymbolTable.hpp>
#include <mirv/Core/Builder/SymbolTerminals.hpp>
#include <mirv/Core/Builder/TypeGrammar.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
  class Type;

  namespace Builder {
    /// This is the grammar to look up a type.  In the case of
    /// constructed types, see if it already exists in the symbol table
    /// and add it if not.
    struct TypeAccessBuilder : boost::proto::or_<
      boost::proto::when<
        StringTerminal,
        LookupSymbol<Type>(
          boost::proto::_data,
          boost::proto::_value)
        >,
      TypeLookupBuilder
      > {};

    /// Translate a Function type string representation to the
    /// corresponding Function type.
    struct FunctionTypeAccessBuilder : boost::proto::or_<
      boost::proto::when<
        StringTerminal,
        LookupSymbol<Type>(
          boost::proto::_data,
          boost::proto::_value)
        >,
      FunctionTypeBuilder
      > {};
  }
}

#endif
