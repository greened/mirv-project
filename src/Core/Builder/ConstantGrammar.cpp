#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
#include <mirv/Core/IR/PlaceholderType.hpp>

#include <mirv/Core/Builder/ConstantGrammar.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      ptr<Symbol<Type<TypeBase> > >::const_type
      IntegralTypeGen::operator()(ptr<SymbolTable>::type symtab,
                                  size_t bitsize)
      {
        return UnaryConstructSymbol<Symbol<Type<Integral> > >()(
          symtab,
          bitsize);
      }

      ptr<Symbol<Type<TypeBase> > >::const_type
      FloatingTypeGen::operator()(ptr<SymbolTable>::type symtab,
                                  size_t bitsize)
      {
        return UnaryConstructSymbol<Symbol<Type<Floating> > >()(
          symtab,
          bitsize);
      }
    }
  }
}
