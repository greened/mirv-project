#ifndef mirv_core_ir_variable_hh
#define mirv_core_ir_variable_hh

namespace mirv {
   class Variable {
   private:
      typedef LeafSymbol interface_base_type

   public:
      class interface
            : public interface_base_type {
      private:
         std::string name;

         typedef ptr<BaseType>::const_type const_type_ptr;
         const_type_ptr the_type;

      public:
         interface(const std::string &n,
                   const_type_ptr t)
               : name(n), the_type(t) {};

         const std::string &name(void) const {
            return(name);
         }

         const_type_ptr type(void) const {
            return(the_type);
         }
      };
   };
}

#endif
