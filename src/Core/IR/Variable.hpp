#ifndef mirv_Core_IR_Variable_hpp
#define mirv_Core_IR_Variable_hpp

#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  /// This is a symbol tag for variable symbols.  Variables have a
  /// type and a name.
   class Variable {
   private:
     class Interface : public Symbol<Typed>,
		       public Symbol<Named>,
		       public LeafSymbol {
      public:
       Interface(const std::string &n,
		 ConstTypePtr t)
	   : Symbol<Typed>(t), Symbol<Named>(n){};

       virtual void accept(mirv::SymbolVisitor &) {
	 error("Variable::Base::accept called");
       }
     };

   public:
     typedef Interface BaseType;
     typedef LeafSymbol VisitorBaseType;
   };
}

#endif
