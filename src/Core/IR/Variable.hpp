#ifndef mirv_Core_IR_Variable_hpp
#define mirv_Core_IR_Variable_hpp

#include <mirv/Core/IR/VariableFwd.hpp>
#include <mirv/Core/IR/Type.hpp>

namespace mirv {
  namespace detail {
    class VariableInterface : public Symbol<Typed>,
                              public Symbol<Named>,
                              public LeafSymbol,
                              public boost::enable_shared_from_this<Symbol<Variable> > {
    public:
      VariableInterface(const std::string &n, TypePtr t)
          : Symbol<Typed>(t), Symbol<Named>(n){}

      ptr<Node<Base>> getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      }
      ptr<const Node<Base>> getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(shared_from_this());
      }

      // We need these to be the final overriders for Symbol<Named>
      // and Symbol<Typed> Visitable::accept functions.
      virtual void accept(SymbolVisitor &) {
        error("Variable::Interface::accept called!");
      }
      virtual void accept(ConstSymbolVisitor &) const {
        error("Variable::Interface::accept called!");
      }
    };
  }

  /// This is a symbol tag for variable symbols.  Variables have a
  /// type and a name.
   class Variable {
   public:
     static void initialize(ptr<Symbol<Variable> > variable) {}

     static std::string getName(const std::string &name,
                                ptr<const Symbol<Type<TypeBase> > > type) {
       return name;
     }
   };
}

#endif
