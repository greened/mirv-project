#ifndef mirv_Core_IR_Constant_hpp
#define mirv_Core_IR_Constant_hpp

#include <boost/lexical_cast.hpp>

#include <mirv/Core/IR/Symbol.hpp>

namespace mirv {
  struct SymbolVisitor;

  /// This is the type implementation for all constant symbols.  It is
  /// a templated Symbol tag.  Each type is an instance of this
  /// template (Symbol<Constant<Integral, int64t> >,
  /// Symbol<Constant<Floating, double> >, etc.).  It keeps all of the
  /// property and visitor logic in one place, hiding the gory details
  /// from the type tags and specific type interfaces.
  template<typename ValueType> class Constant;

  template<>
  class Constant<Base> {
  public:
    typedef LeafSymbol VisitorBaseType;

  private:
    class Interface
        : public Symbol<Typed>,
          public LeafSymbol {
    public:
      Interface(ptr<Symbol<Type<TypeBase> > >::const_type type)
          : Symbol<Typed>(type) {}
      virtual std::string valueString(void) const = 0;
    };

  public:
    typedef Interface BaseType;
  };    

  template<typename ValueType>
  class Constant {
  private:
    class Interface 
        : public Symbol<Constant<Base>>,
          public boost::enable_shared_from_this<Symbol<Constant<ValueType> > > {
    private:
      ValueType val;

    public:
      Interface(typename ptr<Symbol<Type<TypeBase> > >::const_type type,
                ValueType v) : Symbol<Constant<Base>>(type), val(v) {}

      ValueType value(void) const {
        return val;
      }

      std::string valueString(void) const {
        return boost::lexical_cast<std::string>(value());
      }

       ptr<Node<Base>>::type getSharedHandle(void) {
         return fast_cast<Node<Base>>(this->shared_from_this());
       };
       ptr<Node<Base>>::const_type getSharedHandle(void) const {
         return fast_cast<const Node<Base>>(this->shared_from_this());
       };
    };

  public:
    typedef Interface BaseType;
    typedef Symbol<Constant<Base>> VisitorBaseType;
  };
}

#endif
