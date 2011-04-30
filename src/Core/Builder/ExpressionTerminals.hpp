#ifndef mirv_Core_Builder_Expression_Terminals_hpp
#define mirv_Core_Builder_Expression_Terminals_hpp

#include <mirv/Core/Builder/Keywords.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
     /// This is a wrapper so that the grammar can distinguish various
     /// symbol terminals.  We need this so the grammar knows which
     /// kind of symbol to look up.  For example, "foo" could refer to
     /// a variable or a function.  Including the symbol type in the
     /// terminal definition allows the grammar to disambiguate symbol
     /// references.
     template<typename Symbol>
     class SymbolTerminal {
     private:
       std::string name;

     public:
       SymbolTerminal(const std::string &n) : name(n) {}
       SymbolTerminal(const char *n) : name(n) {}
       operator const std::string &(void) const {
         return name;
       }

       friend std::ostream& operator<<(std::ostream& sout, SymbolTerminal<Symbol> sym) {
         return sout << std::string(sym);
       }
     };

     /// Define a convenient way to talk about variable reference
     /// expressions.
     typedef Wrapper<boost::proto::terminal<SymbolTerminal<Variable>>::type>
     VariableTerminal;

     /// Define a convenient way to talk about global variable
     /// reference expressions.
     typedef Wrapper<boost::proto::terminal<SymbolTerminal<GlobalVariable>>::type>
     GlobalVariableTerminal;

     /// Define a convenient way to talk about function reference
     /// expressions.
     typedef Wrapper<boost::proto::terminal<SymbolTerminal<Function>>::type>
     FunctionTerminal;
   }
}

#endif
