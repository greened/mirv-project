#ifndef mirv_Core_Builder_SymbolTableFwd_hpp
#define mirv_Core_Builder_SymbolTableFwd_hpp

namespace mirv {
  namespace Builder {
    /// This is a proto object transform to hold references to the
    /// created module and funceion symbols.  This allows children to
    /// query for various symbols as needed.
    class SymbolTable;
  }
}

#endif
