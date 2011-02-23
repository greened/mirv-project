#include <mirv/Core/Builder/ConstantGrammar.hpp>
#include <mirv/Core/Builder/SymbolTransforms.hpp>
#include <mirv/Core/IR/FloatingType.hpp>
#include <mirv/Core/IR/IntegralType.hpp>
// FIXME: These shouldn't be necessary.  Investigate with a better
// compiler.
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      ptr<Symbol<Type<Integral> > >::const_type
      IntegralTypeGen::operator()(ptr<SymbolTable>::type symtab,
                                  size_t bitsize)
      {
        return UnaryConstructSymbol<Symbol<Type<Integral> > >()(
          symtab,
          bitsize);
      }

      ptr<Symbol<Type<Floating> > >::const_type
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
