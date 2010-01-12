#ifndef mirv_core_ir_variable_hh
#define mirv_core_ir_variable_hh

#include <mirv/core/ir/type.hh>

namespace mirv {
   class Variable {
   public:
      class interface
	: public Symbol<Typed>,
	  public Symbol<Named> {
      public:
         interface(const std::string &n,
                   const_type_ptr t)
	   : Symbol<Typed>(t), Symbol<Named>(n){};
      };
     typedef interface base_type;
     typedef Symbol<Typed> visitor_base_type;
   };
}

#endif
