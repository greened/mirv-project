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
     
     typedef InnerImpl<Statement<Base>, VisitedInherit1<StatementVisitor>::apply<Virtual<Symbol<Base> > >::type> StatementBaseType;
     typedef InnerImpl<Symbol<Variable>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> VariableBaseType;

   public:
     class interface : public NamedBaseType,
		       public TypedBaseType,
		       public VariableBaseType,
		       public StatementBaseType {
     public:
       typedef StatementBaseType::child_ptr statement_ptr;
       typedef VariableBaseType::child_ptr VariablePtr;

       interface(const std::string &n,
		 const_type_ptr t,
		 statement_ptr s)
	 : NamedBaseType(n),
	   TypedBaseType(t),
	   // If the statement is not a block, make it one.
	   StatementBaseType(dyn_cast<Statement<Block> >(s) ?
			     s : mirv::make<Statement<Block> >(s)) {};

       void variablesPushBack(VariablePtr v) {
	 VariableBaseType::push_back(v);
       }

       typedef VariableBaseType::iterator variable_iterator;
       typedef VariableBaseType::const_iterator const_variable_iterator;
       variable_iterator variable_begin(void) {
	 return VariableBaseType::begin();
       }
       variable_iterator variable_end(void) {
	 return VariableBaseType::end();
       }
       const_variable_iterator variable_begin(void) const {
	 return VariableBaseType::begin();
       }
       const_variable_iterator variable_end(void) const {
	 return VariableBaseType::end();
       }

       statement_ptr getStatement(void) {
	 return *StatementBaseType::begin();
       }

       virtual void accept(mirv::SymbolVisitor &) {
	 error("Function::Base::accept called");
       }
     };
     typedef interface base_type;
     typedef TypedBaseType visitor_base_type;
   };
}

#endif
