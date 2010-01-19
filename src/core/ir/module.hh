#ifndef mirv_core_ir_module_hh
#define mirv_core_ir_module_hh

#include <mirv/core/ir/function.hh>
#include <mirv/core/ir/variable.hh>

namespace mirv {
  /// This is the symbol tag for module symbols.  A module is a
  /// collection of types, global variables and functions packaged
  /// together.
   class Module {
   private:
     typedef InnerImpl<Symbol<Function>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> FunctionBaseType;
     typedef InnerImpl<Symbol<Type<TypeBase> >, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> TypeBaseType;
     typedef InnerImpl<Symbol<Variable>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> VariableBaseType;

     class interface : public Symbol<Named>,
                       public FunctionBaseType,
		       public TypeBaseType,
		       public VariableBaseType {
     public:
       interface(const std::string &n) : Symbol<Named>(n) {}

       // Access function information
       typedef FunctionBaseType::child_ptr FunctionPointer;

       /// Add a function.
       void functionsPushBack(FunctionPointer f) {
	 FunctionBaseType::push_back(f);
       }
 
       typedef FunctionBaseType::iterator function_iterator;
       typedef FunctionBaseType::const_iterator const_function_iterator;
       /// Return the start of the function sequence.
       function_iterator function_begin(void) {
	 return FunctionBaseType::begin();
       }
       /// Return the end of the function sequence.
       function_iterator function_end(void) {
	 return FunctionBaseType::end();
       }
       /// Return the start of the function sequence.
       const_function_iterator function_begin(void) const {
	 return FunctionBaseType::begin();
       }
       /// Return the end of the function sequence.
       const_function_iterator function_end(void) const {
	 return FunctionBaseType::end();
       }

       // Access variable information
       typedef VariableBaseType::child_ptr VariablePointer;

       /// Add a global variable.
       void variablesPushBack(VariablePointer v) {
	 VariableBaseType::push_back(v);
       }

       typedef VariableBaseType::iterator variable_iterator;
       typedef VariableBaseType::const_iterator const_variable_iterator;
       /// Return the start of the variable sequence.
       variable_iterator variable_begin(void) {
	 return VariableBaseType::begin();
       }
       /// Return the end of the variable sequence.
       variable_iterator variable_end(void) {
	 return VariableBaseType::end();
       }
       /// Return the start of the variable sequence.
       const_variable_iterator variable_begin(void) const {
	 return VariableBaseType::begin();
       }
       /// Return the end of the variable sequence.
       const_variable_iterator variable_end(void) const {
	 return VariableBaseType::end();
       }

       // Access type information
       typedef TypeBaseType::child_ptr TypePointer;

       /// Add a type.
       void typesPushBack(TypePointer t) {
	 TypeBaseType::push_back(t);
       }

       typedef TypeBaseType::iterator type_iterator;
       typedef TypeBaseType::const_iterator const_type_iterator;
       /// Return the start of the type sequence.
       type_iterator type_begin(void) {
	 return TypeBaseType::begin();
       }
       /// Return the end of the type sequence.
       type_iterator type_end(void) {
	 return TypeBaseType::end();
       }
       /// Return the start of the type sequence.
       const_type_iterator type_begin(void) const {
	 return TypeBaseType::begin();
       }
       /// Return the end of the type sequence.
       const_type_iterator type_end(void) const {
	 return TypeBaseType::end();
       }

       virtual void accept(mirv::SymbolVisitor &) {
	 error("Module::Base::accept called");
       }
     };
   public:
     typedef interface base_type;
     typedef FunctionBaseType visitor_base_type;
   };
}

#endif
