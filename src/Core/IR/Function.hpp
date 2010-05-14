#ifndef mirv_Core_IR_Function_hpp
#define mirv_Core_IR_Function_hpp

#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Statement.hpp>

#include <tr1/functional>
//#include <boost/bind.hpp>

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
		 ConstTypePtr t)
	 : NamedBaseType(n),
	   TypedBaseType(t) {}

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

       VariableIterator variableFind(const std::string &name) {
	 return std::find_if(variableBegin(), variableEnd(),
			     std::tr1::bind(SymbolByName<Variable>(), std::tr1::placeholders::_1, name));
	 //boost::bind(SymbolByName<Symbol<Variable> >(), _1, name));
       }

       void statementPushBack(StatementPtr stmt) {
	 // If the statement is not a block, make it one.
	 if (StatementBaseType::empty())  {
	   StatementPtr newStmt = (dyn_cast<Statement<Block> >(stmt) ?
				   stmt : mirv::make<Statement<Block> >(stmt));
	   StatementBaseType::push_back(newStmt);
	 }
	 else {
	   ptr<Statement<Block> >::type block =
	     safe_cast<Statement<Block> >(StatementBaseType::front());
	   block->push_back(stmt);
	 }
       }

       /// Get the single block statement child.
       StatementPtr getStatement(void) {
	 return *StatementBaseType::begin();
       }

       /// Return whether the function does not have a statement.
       bool statementEmpty(void) const {
	 return StatementBaseType::empty();
       }

       virtual void accept(mirv::SymbolVisitor &) {
	 error("Function::Base::accept called");
       }
     };
     typedef Interface BaseType;
     typedef TypedBaseType VisitorBaseType;

     static std::string getName(const std::string &name,
                                ptr<Symbol<Type<TypeBase> > >::type type) {
       return name;
     }
   };
}

#endif