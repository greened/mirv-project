#ifndef mirv_core_ir_module_hh
#define mirv_core_ir_module_hh

#include <mirv/core/ir/function.hh>
#include <mirv/core/ir/variable.hh>

namespace mirv {
   class Module {
   private:
     typedef InnerImpl<Symbol<Function>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> FunctionBaseType;
     typedef InnerImpl<Symbol<Type<Base> >, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> TypeBaseType;
     typedef InnerImpl<Symbol<Variable>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> VariableBaseType;

     class interface : public Symbol<Named>,
                       public FunctionBaseType,
		       public TypeBaseType,
		       public VariableBaseType {
     public:
       interface(const std::string &n) : Symbol<Named>(n) {}
     };
   public:
     typedef interface base_type;
     typedef FunctionBaseType visitor_base_type;
   };
}

#endif
