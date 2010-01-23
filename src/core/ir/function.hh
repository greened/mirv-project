#ifndef mirv_core_ir_function_hh
#define mirv_core_ir_function_hh

#include <mirv/core/ir/symbol.hh>
#include <mirv/core/ir/variable.hh>
#include <mirv/core/ir/statement.hh>

namespace mirv {
  /// This is a symbol tag for function symbols.
   class Function {
   private:
     typedef Symbol<Named> NamedBaseType;
     typedef Symbol<Typed> TypedBaseType;
     
     typedef InnerImpl<Statement<Base>, VisitedInherit1<StatementVisitor>::apply<Virtual<Symbol<Base> > >::type> StatementBaseType;
     typedef InnerImpl<Symbol<Variable>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> VariableBaseType;

   public:
     class Interface : public NamedBaseType,
		       public TypedBaseType,
		       public VariableBaseType,
		       public StatementBaseType {
     public:
       typedef StatementBaseType::ChildPtr StatementPtr;
       typedef VariableBaseType::ChildPtr VariablePtr;

       Interface(const std::string &n,
		 ConstTypePtr t,
		 StatementPtr s)
	 : NamedBaseType(n),
	   TypedBaseType(t),
	   // If the statement is not a block, make it one.
	   StatementBaseType(dyn_cast<Statement<Block> >(s) ?
			     s : mirv::make<Statement<Block> >(s)) {};

       /// Add a local variable to this function.
       void variablePushBack(VariablePtr v) {
	 VariableBaseType::push_back(v);
       }

       typedef VariableBaseType::iterator VariableIterator;
       typedef VariableBaseType::const_iterator ConstVariableIterator;
       /// Get the start of the local variable sequence.
       VariableIterator variableBegin(void) {
	 return VariableBaseType::begin();
       }
       /// Get the end of the local variable sequence.
       VariableIterator variableEnd(void) {
	 return VariableBaseType::end();
       }
       /// Get the start of the local variable sequence.
       ConstVariableIterator variableBegin(void) const {
	 return VariableBaseType::begin();
       }
       /// Get the end of the local variable sequence.
       ConstVariableIterator variableEnd(void) const {
	 return VariableBaseType::end();
       }

       /// Get the single block statement child.
       StatementPtr getStatement(void) {
	 return *StatementBaseType::begin();
       }

       virtual void accept(mirv::SymbolVisitor &) {
	 error("Function::Base::accept called");
       }
     };
     typedef Interface BaseType;
     typedef TypedBaseType VisitorBaseType;
   };
}

#endif
