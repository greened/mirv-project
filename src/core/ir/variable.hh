#ifndef mirv_core_ir_variable_hh
#define mirv_core_ir_variable_hh

#include <mirv/core/ir/type.hh>

namespace mirv {
   class Variable {
   private:
     typedef LeafSymbol interface_base_type;

   public:
      class interface
            : public interface_base_type {
      private:
         std::string theName;

	typedef ptr<Symbol<Type<TypeBase> > >::const_type const_type_ptr;
         const_type_ptr the_type;

      public:
         interface(const std::string &n,
                   const_type_ptr t)
               : theName(n), the_type(t) {};

         const std::string &name(void) const {
            return(theName);
         }

         const_type_ptr type(void) const {
            return(the_type);
         }
      };
     typedef interface base_type;
   };
}

#endif
