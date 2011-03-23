#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/TupleType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>

#include <mirv/Core/Builder/SymbolTerminals.hpp>
#include <mirv/Core/Builder/TypeGrammar.hpp>
#include <mirv/Core/Builder/ArrayTypeGrammar.hpp>
#include <mirv/Core/Builder/MakeExpression.hpp>

namespace mirv {
  namespace Builder {
    void
    ConstructStructTypeSymbol::
    resolve(boost::shared_ptr<SymbolTable> symtab,
            const std::string &oldName,
            ptr<Symbol<Type<Placeholder> > >::const_type placeholder,
            ptr<Symbol<Type<TypeBase> > >::const_type replacement) {
      // This isn't in the symbol table yet, so add it now.  This
      // avoids the need for a definition for Tuple to do
      // placeholder resolution on it.
      LookupAndAddSymbol<Symbol<Type<TypeBase> > >()(symtab, replacement);

      // Resolve any types referencing this placeholder, including
      // this tuple.
      symtab->resolve(oldName, placeholder, replacement);
    }

    ptr<Symbol<Type<TypeBase> > >::const_type
    LookupPlaceholder::operator()(boost::shared_ptr<SymbolTable> symtab,
                                  const std::string &name)
    {
      auto placeholder = symtab->lookupPlaceholder(name);
      checkInvariant(placeholder, "Missing placeholder!");
      return placeholder;
    }
  }
}
