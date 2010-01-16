#ifndef mirv_core_ir_variable_hh
#define mirv_core_ir_variable_hh

#include <mirv/core/ir/type.hh>

namespace mirv {
   class Variable {
   public:
      class interface
	: public Symbol<Typed>,
	  public Symbol<Named>,
	  public LeafSymbol {
      public:
         interface(const std::string &n,
                   const_type_ptr t)
	   : Symbol<Typed>(t), Symbol<Named>(n){};

	virtual void accept(mirv::SymbolVisitor &) {
	  error("Variable::Base::accept called");
	}
      };
     typedef interface base_type;
     typedef LeafSymbol visitor_base_type;
   };
}

#endif
