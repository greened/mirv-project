#ifndef mirv_Core_IR_Module_hpp
#define mirv_Core_IR_Module_hpp

#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>

namespace mirv {
  /// This is the symbol tag for module symbols.  A module is a
  /// collection of types, global variables and functions packaged
  /// together.
   class Module {
   private:
     typedef InnerImpl<Symbol<Function>, Virtual<Symbol<Base> > > FunctionBaseType;
     typedef InnerImpl<const Symbol<Type<TypeBase> >, Virtual<Symbol<Base> > > TypeBaseType;
     typedef InnerImpl<Symbol<GlobalVariable>, Virtual<Symbol<Base> > > GlobalVariableBaseType;

     class Interface : public Symbol<Named>,
                       public FunctionBaseType,
		       public TypeBaseType,
		       public GlobalVariableBaseType,
                       public boost::enable_shared_from_this<Symbol<Module> > {
     public:
       Interface(const std::string &n);

       // Access function information
       typedef FunctionBaseType::ChildPtr FunctionPointer;

       /// Add a function.
       void functionPushBack(FunctionPointer f) {
	 FunctionBaseType::push_back(f);
         f->setParent(this->getSharedHandle());
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
  ConstSymbolVisitor::result_type
  ConstSymbolVisitor::visit(ptr<Symbol<Variable> >::const_type s) {
    ptr<Symbol<Variable>::VisitorBaseType>::const_type p =
      fast_cast<const Symbol<Variable>::VisitorBaseType>(s);
    visit(p);
  }

       // Access variable information
       typedef GlobalVariableBaseType::ChildPtr GlobalVariablePointer;

       /// Add a global variable.
       void globalVariablePushBack(GlobalVariablePointer v) {
	 GlobalVariableBaseType::push_back(v);
         v->setParent(this->getSharedHandle());
       }

       typedef GlobalVariableBaseType::iterator GlobalVariableIterator;
       typedef GlobalVariableBaseType::const_iterator ConstGlobalVariableIterator;
       /// Return the start of the variable sequence.
       GlobalVariableIterator globalVariableBegin(void) {
	 return GlobalVariableBaseType::begin();
       }
       /// Return the end of the variable sequence.
       GlobalVariableIterator globalVariableEnd(void) {
	 return GlobalVariableBaseType::end();
       }
       /// Return the start of the variable sequence.
       ConstGlobalVariableIterator globalVariableBegin(void) const {
	 return GlobalVariableBaseType::begin();
       }
       /// Return the end of the variable sequence.
       ConstGlobalVariableIterator globalVariableEnd(void) const {
	 return GlobalVariableBaseType::end();
       }

       GlobalVariableIterator globalVariableFind(const std::string &name) {
	 return std::find_if(globalVariableBegin(), globalVariableEnd(),
			     std::tr1::bind(SymbolByName<GlobalVariable>(), std::tr1::placeholders::_1, name));
       }

       // Access type information
       typedef TypeBaseType::ConstChildPtr TypePointer;

       /// Add a type.
       void typePushBack(TypePointer t) {
	 TypeBaseType::push_back(t);
         t->setParent(this->getSharedHandle());
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

       ptr<Node<Base>>::type getSharedHandle(void) {
         return fast_cast<Node<Base>>(shared_from_this());
       }
       ptr<Node<Base>>::const_type getSharedHandle(void) const {
         return fast_cast<const Node<Base>>(shared_from_this());
       }
     };
   public:
     typedef Interface BaseType;
     typedef FunctionBaseType VisitorBaseType;

     static void initialize(ptr<Symbol<Module> >::type module);

     static std::string getName(const std::string &name) {
       return name;
     }
   };
}

#endif
