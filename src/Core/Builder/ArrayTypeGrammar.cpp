#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Symbol.hpp>

#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/SymbolTerminals.hpp>
#include <mirv/Core/Builder/TypeGrammar.hpp>
#include <mirv/Core/Builder/ArrayTypeGrammar.hpp>
#include <mirv/Core/Builder/MakeExpression.hpp>

namespace mirv {
  namespace Builder {
    ptr<const Type>
    ConstructArrayTypeSymbol::operator()(ptr<SymbolTable> symtab,
                                         detail::TypeSubscriptData subscripts) {
      // Construct a nested tuple type.
      ptr<const Type> elementType =
        subscripts.elementType();

      auto s = subscripts.begin();

      result_type Tuple =
        BinaryConstructSymbol<TupleType, ModuleScope>()(
          symtab,
          elementType,
          makeExpression(*s, IRBuilder::getCurrentModule()));

      while (++s != subscripts.end()) {
        Tuple =
          BinaryConstructSymbol<TupleType, ModuleScope>()(
            symtab,
            Tuple,
            makeExpression(*s, IRBuilder::getCurrentModule()));
      }

      return Tuple;
    }
  }
}
