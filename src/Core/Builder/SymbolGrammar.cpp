#include <mirv/Core/Builder/SymbolGrammar.hpp>

namespace mirv {
  namespace Builder {
    namespace detail {
      const char *GetBaseName<Symbol<Type<Integral> > >::value = "int";
      const char *GetBaseName<Symbol<Type<Floating> > >::value = "float";
    }
  }
}
