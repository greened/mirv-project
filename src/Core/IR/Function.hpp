#ifndef mirv_Core_IR_Function_hpp
#define mirv_Core_IR_Function_hpp

#include <mirv/Core/IR/StatementFwd.hpp>
#include <mirv/Core/IR/Symbol.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/VariableFwd.hpp>

#include <tr1/functional>

namespace mirv {
  /// This is a symbol tag for function symbols.
   class Function {
   private:
     typedef Symbol<Named> NamedBaseType;
     typedef Symbol<Typed> TypedBaseType;
     
     typedef InnerImpl<
       Statement<Base>,
       Virtual<Symbol<Base> >
       > StatementBaseType;

     typedef InnerImpl<
       Symbol<Variable>,
       Virtual<Symbol<Base> >
       > VariableBaseType;

   public:
     class Interface : public NamedBaseType,
		       public TypedBaseType,
		       public VariableBaseType,
		       public StatementBaseType,
                       public boost::enable_shared_from_this<Symbol<Function> > {
     public:
       typedef StatementBaseType::ChildPtr StatementPtr;
       typedef StatementBaseType::ConstChildPtr ConstStatementPtr;
       typedef VariableBaseType::ChildPtr VariablePtr;
       typedef VariableBaseType::ConstChildPtr ConstVariablePtr;

       Interface(const std::string &n, TypePtr t)
	 : NamedBaseType(n), TypedBaseType(t) {}

       Interface(const std::string &n,
		 TypePtr t,
		 StatementPtr s);

       /// Add a local variable to this function.
       void variablePushBack(VariablePtr v);

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

       void statementPushBack(StatementPtr stmt);

       /// Get the single block statement child.
       StatementPtr getStatement(void) {
	 return *StatementBaseType::begin();
       }
       ConstStatementPtr getStatement(void) const {
	 return *StatementBaseType::begin();
       }

       /// Return whether the function does not have a statement.
       bool statementEmpty(void) const {
	 return StatementBaseType::empty();
       }

       ptr<Node<Base>>::type getSharedHandle(void) {
         return fast_cast<Node<Base>>(shared_from_this());
       }
       ptr<Node<Base>>::const_type getSharedHandle(void) const {
         return fast_cast<const Node<Base>>(shared_from_this());
       }

       // We need these to be the final overriders for
       // Visitable::accept functions.
       virtual void accept(SymbolVisitor &) {
         error("Function::Interface::accept called!");
       }
       virtual void accept(ConstSymbolVisitor &) const {
         error("Function::Interface::accept called!");
       }
     };
     typedef Interface BaseType;
     typedef TypedBaseType VisitorBaseType;

     static std::string getName(const std::string &name,
                                ptr<Symbol<Type<TypeBase> > >::const_type type) {
       return name;
     }
   };
}

#endif
