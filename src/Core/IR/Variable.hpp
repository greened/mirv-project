#ifndef mirv_Core_IR_Variable_hpp
#define mirv_Core_IR_Variable_hpp

#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  /// This is a symbol tag for variable symbols.  Variables have a
  /// type and a name.
   class Variable {
   private:
     class Interface : public Symbol<Typed>,
		       public Symbol<Named>,
		       public LeafSymbol,
                       public boost::enable_shared_from_this<Symbol<Variable> > {
      public:
       Interface(const std::string &n, TypePtr t)
	   : Symbol<Typed>(t), Symbol<Named>(n){};

       ptr<Node<Base>>::type getSharedHandle(void) {
         return fast_cast<Node<Base>>(shared_from_this());
       };

       virtual void accept(mirv::SymbolVisitor &) {
	 error("Variable::Base::accept called");
       }
       virtual void accept(mirv::ConstSymbolVisitor &) const {
	 error("Variable::Base::accept called");
       }
     };

   public:
     typedef Interface BaseType;
     typedef LeafSymbol VisitorBaseType;

     static std::string getName(const std::string &name,
                                ptr<Symbol<Type<TypeBase> > >::const_type type) {
       return name;
     }
   };
}

#endif
