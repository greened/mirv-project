#ifndef mirv_core_ir_function_hh
#define mirv_core_ir_function_hh

namespace mirv {
   class Function {
   private:
     typedef Symbol<Named> NamedBaseType;
     typedef Symbol<Typed> TypedBaseType;
     typedef Inner<Statement<Base>, Virtual<InnerSymbol> > StatementBaseType;
     typedef Inner<Symbol<Variable>, Virtual<InnerSymbol> > VariableBaseType;

   public:
     class interface : public NamedBaseType,
		       public TypedBaseType,
		       public VariableBaseType,
		       public StatementBaseType {
     public:
         interface(const std::string &n,
                   const_type_ptr t)
	   : NamedBaseType(n), TypedBaseType(t) {};
     };
   };
}

#endif
