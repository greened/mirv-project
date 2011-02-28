#include <mirv/Core/Builder/SymbolTerminals.hpp>
#include <mirv/Core/Builder/TypeGrammar.hpp>
#include <mirv/Core/Builder/ArrayTypeGrammar.hpp>
#include <mirv/Core/Builder/MakeExpression.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/TupleType.hpp>

namespace mirv {
  namespace Builder {
    ptr<Symbol<Type<Tuple> > >::const_type
    ConstructArrayTypeSymbol::operator()(boost::shared_ptr<SymbolTable> symtab,
                                         detail::TypeSubscriptData subscripts) {
      // Construct a nested tuple type.
      ptr<Symbol<Type<TypeBase> > >::const_type elementType =
        subscripts.elementType();

      auto s = subscripts.begin();

      result_type TupleType =
        BinaryConstructSymbol<Symbol<Type<Tuple> > >()(
          symtab,
          elementType,
          makeExpression(*s, symtab->getModule()));
        
      while (++s != subscripts.end()) {
        TupleType =
          BinaryConstructSymbol<Symbol<Type<Tuple> > >()(
            symtab,
            TupleType,
            makeExpression(*s, symtab->getModule()));
      }

      return TupleType;
    }
  }
}
