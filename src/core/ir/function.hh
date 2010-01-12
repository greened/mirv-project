#ifndef mirv_core_ir_function_hh
#define mirv_core_ir_function_hh

#include <mirv/core/ir/symbol.hh>
#include <mirv/core/ir/variable.hh>
#include <mirv/core/ir/statement.hh>

namespace mirv {
   class Function {
   private:
     typedef Symbol<Named> NamedBaseType;
     typedef Symbol<Typed> TypedBaseType;
     typedef Statement<SingleBlock> StatementBaseType;  // ??
     typedef InnerImpl<Symbol<Variable>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> VariableBaseType;

   public:
     class interface : public NamedBaseType,
		       public TypedBaseType,
		       public VariableBaseType,
		       public StatementBaseType {
       typedef StatementBaseType::child_ptr statement_ptr;

     public:
         interface(const std::string &n,
                   const_type_ptr t,
		   statement_ptr s)
	   : NamedBaseType(n),
	     TypedBaseType(t),
	     StatementBaseType(s) {};
     };
     typedef interface base_type;
     typedef TypedBaseType visitor_base_type;
   };
}

#endif
