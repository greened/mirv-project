#ifndef mirv_Core_IR_GlobalVariable_hpp
#define mirv_Core_IR_GlobalVariable_hpp

#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  /// This is a symbol tag for variable symbols.  Variables have a
  /// type and a name.
   class GlobalVariable {
   private:
     class Interface;

   public:
     typedef Interface BaseType;

   private:
     // GlobalVariables do not have an isa relationship with
     // Variables.  Variables are single-assignment only while
     // GlobalVariable represents a global region of memory which may
     // be defined multiple times.
     class Interface : public Symbol<Typed>,
		       public Symbol<Named>,
		       public LeafSymbol,
                       public boost::enable_shared_from_this<Symbol<Variable> > {
     private:
       typedef ptr<Symbol<Constant<Base> > >::ConstantPtr;
       ConstantPtr init;

      public:
       Interface(const std::string &n, TypePtr t, ConstantPtr i = ConstantPtr())
	   : Symbol<Typed>(t), Symbol<Named>(n), init(i) {}

       ConstantPtr initializer(void) const {
         return init;
       }

       ptr<Node<Base>>::type getSharedHandle(void) {
         return fast_cast<Node<Base>>(shared_from_this());
       }
       ptr<Node<Base>>::const_type getSharedHandle(void) const {
         return fast_cast<const Node<Base>>(shared_from_this());
       }

       // We need these to be the final overriders for Symbol<Named>
       // and Symbol<Typed> Visitable::accept functions.
       virtual void accept(SymbolVisitor &) {
         error("GlobalVariable::Interface::accept called!");
       }
       virtual void accept(ConstSymbolVisitor &) const {
         error("GlobalVariable::Interface::accept called!");
       }
     };

   public:
     typedef LeafSymbol VisitorBaseType;

     static void initialize(ptr<Symbol<Variable> >::type variable) {}

     static std::string getName(const std::string &name,
                                ptr<Symbol<Type<TypeBase> > >::const_type type) {
       return name;
     }
   };
}

#endif
