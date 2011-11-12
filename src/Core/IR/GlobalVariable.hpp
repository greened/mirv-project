#ifndef mirv_Core_IR_GlobalVariable_hpp
#define mirv_Core_IR_GlobalVariable_hpp

#include <mirv/Core/IR/GlobalVariableFwd.hpp>
#include <mirv/Core/IR/ConstantFwd.hpp>
#include <mirv/Core/IR/ReferenceFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/ConstantFwd.hpp>
#include <mirv/Core/IR/Type.hpp>

namespace mirv {

  namespace detail {
    // GlobalVariables do not have an isa relationship with
    // Variables.  Variables are single-assignment only while
    // GlobalVariable represents a global region of memory which may
    // be defined multiple times.
    class GlobalVariableInterface
        : public Symbol<Global>,
          public LeafSymbol,
          public boost::enable_shared_from_this<Symbol<GlobalVariable> > {
    private:
      typedef ptr<Expression<Reference<Constant<Base> > > >::type ConstantPtr;
      ConstantPtr init;

    public:
      GlobalVariableInterface(const std::string &n, TypePtr t, ConstantPtr i = ConstantPtr())
          : Symbol<Global>(n, t), init(i) {}

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
  }

  /// This is a symbol tag for variable symbols.  Variables have a
  /// type and a name.
   class GlobalVariable {
   public:
     static void initialize(ptr<Symbol<GlobalVariable> >::type variable) {}

     static std::string
     getName(const std::string &name,
             ptr<Symbol<Type<TypeBase> > >::const_type type) {
       return name;
     }
     template<typename Init>
     static std::string
     getName(const std::string &name,
             ptr<Symbol<Type<TypeBase> > >::const_type type,
             const Init &) {
       return name;
     }
   };
}

#endif
