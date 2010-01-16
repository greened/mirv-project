#ifndef mirv_core_ir_module_hh
#define mirv_core_ir_module_hh

#include <mirv/core/ir/function.hh>
#include <mirv/core/ir/variable.hh>

namespace mirv {
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

       void functionsPushBack(FunctionPointer f) {
	 FunctionBaseType::push_back(f);
       }
 
       typedef FunctionBaseType::iterator function_iterator;
       typedef FunctionBaseType::const_iterator const_function_iterator;
       function_iterator function_begin(void) {
	 return FunctionBaseType::begin();
       }
       function_iterator function_end(void) {
	 return FunctionBaseType::end();
       }
       const_function_iterator function_begin(void) const {
	 return FunctionBaseType::begin();
       }
       const_function_iterator function_end(void) const {
	 return FunctionBaseType::end();
       }

       // Access variable information
       typedef VariableBaseType::child_ptr VariablePointer;

       void variablesPushBack(VariablePointer v) {
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

       // Access type information
       typedef TypeBaseType::child_ptr TypePointer;

       void typesPushBack(TypePointer t) {
	 TypeBaseType::push_back(t);
       }

       typedef TypeBaseType::iterator type_iterator;
       typedef TypeBaseType::const_iterator const_type_iterator;
       type_iterator type_begin(void) {
	 return TypeBaseType::begin();
       }
       type_iterator type_end(void) {
	 return TypeBaseType::end();
       }
       const_type_iterator type_begin(void) const {
	 return TypeBaseType::begin();
       }
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
