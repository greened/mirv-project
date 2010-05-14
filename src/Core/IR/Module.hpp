#ifndef mirv_Core_IR_Module_hpp
#define mirv_Core_IR_Module_hpp

#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/Variable.hpp>

namespace mirv {
  /// This is the symbol tag for module symbols.  A module is a
  /// collection of types, global variables and functions packaged
  /// together.
   class Module {
   private:
     typedef InnerImpl<Symbol<Function>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> FunctionBaseType;
     typedef InnerImpl<Symbol<Type<TypeBase> >, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> TypeBaseType;
     typedef InnerImpl<Symbol<Variable>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> VariableBaseType;

     class Interface : public Symbol<Named>,
                       public FunctionBaseType,
		       public TypeBaseType,
		       public VariableBaseType {
     public:
       Interface(const std::string &n) : Symbol<Named>(n) {}

       // Access function information
       typedef FunctionBaseType::ChildPtr FunctionPointer;

       /// Add a function.
       void functionPushBack(FunctionPointer f) {
	 FunctionBaseType::push_back(f);
       }
 
       typedef FunctionBaseType::iterator FunctionIterator;
       typedef FunctionBaseType::const_iterator ConstFunctionIterator;
       /// Return the start of the function sequence.
       FunctionIterator functionBegin(void) {
	 return FunctionBaseType::begin();
       }
       /// Return the end of the function sequence.
       FunctionIterator functionEnd(void) {
	 return FunctionBaseType::end();
       }
       /// Return the start of the function sequence.
       ConstFunctionIterator functionBegin(void) const {
	 return FunctionBaseType::begin();
       }
       /// Return the end of the function sequence.
       ConstFunctionIterator functionEnd(void) const {
	 return FunctionBaseType::end();
       }

       FunctionIterator functionFind(const std::string &name) {
	 return std::find_if(functionBegin(), functionEnd(),
			     std::tr1::bind(SymbolByName<Function>(), std::tr1::placeholders::_1, name));
       }

       // Access variable information
       typedef VariableBaseType::ChildPtr VariablePointer;

       /// Add a global variable.
       void variablePushBack(VariablePointer v) {
	 VariableBaseType::push_back(v);
       }

       typedef VariableBaseType::iterator VariableIterator;
       typedef VariableBaseType::const_iterator ConstVariableIterator;
       /// Return the start of the variable sequence.
       VariableIterator variableBegin(void) {
	 return VariableBaseType::begin();
       }
       /// Return the end of the variable sequence.
       VariableIterator variableEnd(void) {
	 return VariableBaseType::end();
       }
       /// Return the start of the variable sequence.
       ConstVariableIterator variableBegin(void) const {
	 return VariableBaseType::begin();
       }
       /// Return the end of the variable sequence.
       ConstVariableIterator variableEnd(void) const {
	 return VariableBaseType::end();
       }

       VariableIterator variableFind(const std::string &name) {
	 return std::find_if(variableBegin(), variableEnd(),
			     std::tr1::bind(SymbolByName<Variable>(), std::tr1::placeholders::_1, name));
       }

       // Access type information
       typedef TypeBaseType::ChildPtr TypePointer;

       /// Add a type.
       void typePushBack(TypePointer t) {
	 TypeBaseType::push_back(t);
       }

       typedef TypeBaseType::iterator TypeIterator;
       typedef TypeBaseType::const_iterator ConstTypeIterator;
       /// Return the start of the type sequence.
       TypeIterator typeBegin(void) {
	 return TypeBaseType::begin();
       }
       /// Return the end of the type sequence.
       TypeIterator typeEnd(void) {
	 return TypeBaseType::end();
       }
       /// Return the start of the type sequence.
       ConstTypeIterator typeBegin(void) const {
	 return TypeBaseType::begin();
       }
       /// Return the end of the type sequence.
       ConstTypeIterator typeEnd(void) const {
	 return TypeBaseType::end();
       }

       TypeIterator typeFind(const std::string &name) {
	 return std::find_if(typeBegin(), typeEnd(),
			     std::tr1::bind(SymbolByName<Type<TypeBase> >(), std::tr1::placeholders::_1, name));
       }

       virtual void accept(mirv::SymbolVisitor &) {
	 error("Module::Base::accept called");
       }
     };
   public:
     typedef Interface BaseType;
     typedef FunctionBaseType VisitorBaseType;

     static std::string getName(const std::string &name) {
       return name;
     }
   };
}

#endif