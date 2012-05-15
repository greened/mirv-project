#ifndef mirv_Core_IR_Constant_hpp
#define mirv_Core_IR_Constant_hpp

#include <boost/lexical_cast.hpp>

#include <mirv/Core/IR/ConstantFwd.hpp>
#include <mirv/Core/IR/Symbol.hpp>

namespace mirv {
  struct SymbolVisitor;

  namespace detail {
    class BaseConstantInterface
        : public Symbol<Typed>,
          public LeafSymbol {
    public:
      BaseConstantInterface(ptr<const Symbol<Type<TypeBase> > > type)
          : Symbol<Typed>(type) {}
    };
  }

  /// This is the type implementation for all constant symbols.  It is
  /// a templated Symbol tag.  Each type is an instance of this
  /// template (Symbol<Constant<Integral, int64t> >,
  /// Symbol<Constant<Floating, double> >, etc.).  It keeps all of the
  /// property and visitor logic in one place, hiding the gory details
  /// from the type tags and specific type interfaces.
  template<typename ValueType> class Constant;

  /// Define the base class for all constants.
  template<>
  class Constant<Base> {};

  namespace detail {
    template<typename ValueType>
    class ConstantInterface 
        : public Symbol<Constant<Base>>,
          public boost::enable_shared_from_this<Symbol<Constant<ValueType> > > {
    private:
      ValueType val;

    public:
      ConstantInterface(ptr<const Symbol<Type<TypeBase> > > type,
                        ValueType v) : Symbol<Constant<Base>>(type), val(v) {}

      ValueType value(void) const {
        return val;
      }

       ptr<Node<Base>> getSharedHandle(void) {
         return fast_cast<Node<Base>>(this->shared_from_this());
       };
       ptr<const Node<Base>> getSharedHandle(void) const {
         return fast_cast<const Node<Base>>(this->shared_from_this());
       };
    };
  }

  /// This is the generic constant tag.  Given a ValueType, it
  /// represents a constant value of that type.
  template<typename ValueType>
  class Constant {
  public:
    static void
    initialize(ptr<Symbol<Constant<ValueType> > > constant) {}
  };
}

#endif
