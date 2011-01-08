#ifndef mirv_Core_Builder_ExpressionRules_hpp
#define mirv_Core_Builder_ExpressionRules_hpp

#include <mirv/Core/Builder/Transform.hpp>
#include <mirv/Core/Builder/DomainFwd.hpp>
#include <mirv/Core/Builder/GrammarFwd.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Function.hpp>

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

     /// Define a convenient way to talk about function reference
     /// expressions.
     typedef Wrapper<boost::proto::terminal<SymbolTerminal<Function>>::type>
     FunctionTerminal;

     // Unary expressions

     // No Posit
     typedef boost::proto::negate<ConstructExpressionGrammar> NegateRule;
     typedef boost::proto::complement<ConstructExpressionGrammar> ComplementRule;
     typedef boost::proto::dereference<ConstructExpressionGrammar> DereferenceRule;
     typedef boost::proto::address_of<ConstructExpressionGrammar> AddressOfRule;
     typedef boost::proto::logical_not<ConstructExpressionGrammar> NotRule;

      // No increment or decrement

      // Binary expressions
     typedef boost::proto::plus<ConstructExpressionGrammar, ConstructExpressionGrammar> AddRule;
     typedef boost::proto::minus<ConstructExpressionGrammar, ConstructExpressionGrammar> MinusRule;
     typedef boost::proto::multiplies<ConstructExpressionGrammar, ConstructExpressionGrammar> MultipliesRule;
     typedef boost::proto::divides<ConstructExpressionGrammar, ConstructExpressionGrammar> DividesRule;
     typedef boost::proto::shift_left<ConstructExpressionGrammar, ConstructExpressionGrammar> ShiftLeftRule;
     typedef boost::proto::shift_right<ConstructExpressionGrammar, ConstructExpressionGrammar> ShiftRightRule;
     typedef boost::proto::modulus<ConstructExpressionGrammar, ConstructExpressionGrammar> ModulusRule;
     typedef boost::proto::greater<ConstructExpressionGrammar, ConstructExpressionGrammar> GreaterRule;
     typedef boost::proto::less<ConstructExpressionGrammar, ConstructExpressionGrammar> LessRule;
     typedef boost::proto::greater_equal<ConstructExpressionGrammar, ConstructExpressionGrammar> GreaterEqualRule;
     typedef boost::proto::less_equal<ConstructExpressionGrammar, ConstructExpressionGrammar> LessEqualRule;
     typedef boost::proto::equal_to<ConstructExpressionGrammar, ConstructExpressionGrammar> EqualRule;
     typedef boost::proto::not_equal_to<ConstructExpressionGrammar, ConstructExpressionGrammar> NotEqualRule;
     typedef boost::proto::logical_or<ConstructExpressionGrammar, ConstructExpressionGrammar> OrRule;
     typedef boost::proto::logical_and<ConstructExpressionGrammar, ConstructExpressionGrammar> AndRule;
     typedef boost::proto::bitwise_or<ConstructExpressionGrammar, ConstructExpressionGrammar> BitwiseOrRule;
     typedef boost::proto::bitwise_and<ConstructExpressionGrammar, ConstructExpressionGrammar> BitwiseAndRule;
     typedef boost::proto::bitwise_xor<ConstructExpressionGrammar, ConstructExpressionGrammar> BitwiseXorRule;

     typedef boost::proto::mem_ptr<ConstructExpressionGrammar, ConstructExpressionGrammar> MemPtrRule;

     // Nary Rules
     typedef boost::proto::subscript<ConstructExpressionGrammar,
       boost::proto::vararg<ConstructExpressionGrammar> > SubscriptRule;

     // No comma
     // No assign (statement)
     // No op-assign
     // No ?:
   }
}

#endif
