#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Symbol.hpp>

#include <mirv/Core/Builder/SymbolTerminals.hpp>
#include <mirv/Core/Builder/TypeGrammar.hpp>
#include <mirv/Core/Builder/ArrayTypeGrammar.hpp>
#include <mirv/Core/Builder/MakeExpression.hpp>

namespace mirv {
  namespace Builder {
    // void
    // ConstructStructTypeSymbol::
    // resolve(ptr<SymbolTable> symtab,
    //         const std::string &oldName,
    //         ptr<const PlaceholderType> placeholder,
    //         ptr<const Type> replacement) {
    //   // Resolve any types referencing this placeholder, including
    //   // this tuple.
    //   symtab->resolve(oldName, placeholder, replacement);
    // }

    ptr<const Type>
    LookupPlaceholder::operator()(ptr<SymbolTable> symtab,
                                  const std::string &name) {
      auto placeholder = IRBuilder::findPlaceholderType(name);
      checkInvariant(placeholder, "Missing placeholder!");
      return placeholder;
    }
  }
}
