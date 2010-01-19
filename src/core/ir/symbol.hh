#ifndef mirv_core_ir_symbol_hh
#define mirv_core_ir_symbol_hh

#include <mirv/core/ir/base.hh>
#include <mirv/core/ir/node.hh>
#include <mirv/core/ir/type_fwd.hh>

namespace mirv {
  struct SymbolVisitor;

  /// This is the symbol implementation for all symbol types.  Each
  /// symbol type is an instance of this template (Symbol<Variable>,
  /// Symbol<Function>, etc.).  It keeps all of the property and
  /// visitor logic in one place, hiding the gory details from the
  /// symbol type tags and specific symbol type interfaces.
   template<typename Tag>
   class Symbol : public Tag::base_type {
   public:
     typedef typename Tag::base_type base_type;
     typedef typename Tag::visitor_base_type visitor_base_type;

   protected:
     Symbol(void) {}
     template<typename A1>
     Symbol(A1 a1) : base_type(a1) {}
     template<typename A1, typename A2>
     Symbol(A1 a1, A2 a2) : base_type(a1, a2) {}
     template<typename A1, typename A2, typename A3>
     Symbol(A1 a1, A2 a2, A3 a3) : base_type(a1, a2, a3) {}

   public:
     static typename ptr<Symbol<Tag> >::type
     make(void) {
       return typename ptr<Symbol<Tag> >::type(new Symbol<Tag>());
     }

     template<typename A1>
     static typename ptr<Symbol<Tag> >::type
     make(A1 a1) {
       return typename ptr<Symbol<Tag> >::type(new Symbol<Tag>(a1));
     }

     template<typename A1, typename A2>
     static typename ptr<Symbol<Tag> >::type
     make(A1 a1, A2 a2) {
       return typename ptr<Symbol<Tag> >::type(new Symbol<Tag>(a1, a2));
     }

     template<typename A1, typename A2, typename A3>
     static typename ptr<Symbol<Tag> >::type
     make(A1 a1, A2 a2, A3 a3) {
       return typename ptr<Symbol<Tag> >::type(new Symbol<Tag>(a1, a2, a3));
     }

     virtual void accept(SymbolVisitor &V);
   };

  /// A specialization for base symbols.
   template<>
   class Symbol<Base> : public Node<Base> { 
   public:
     virtual void accept(SymbolVisitor &V);
   };

  /// This is the implementation of inner symbols.  It is
  /// inherited from once in the hierarchy for any inner symbols.
  /// This holds the child pointers and other data necessary for inner
  /// symbols.
  class InnerSymbol : public InnerImpl<Symbol<Base>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> {
  public:
    typedef Symbol<Base> visitor_base_type;
    virtual void accept(SymbolVisitor &V);
  };

  /// This is a symbol with no children.
  class LeafSymbol : public LeafImpl<VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> {
  public:
    typedef Symbol<Base> visitor_base_type;
    virtual void accept(SymbolVisitor &V);
  };

  /// A symbol that has a type associated with it.
  class Typed {
  private:
    typedef Inherit1::apply<Virtual<Symbol<Base> > >::type interface_base_type;

  public:
    class interface : public interface_base_type { 
    public:
      typedef ptr<Symbol<Type<TypeBase> > >::const_type const_type_ptr;

   private:
      const_type_ptr the_type;

    public:
      interface(const_type_ptr t)
	: the_type(t) {};

      const_type_ptr type(void) const {
	return(the_type);
      }
      virtual void accept(mirv::SymbolVisitor &) {
	error("Typed::accept called");
      }
    };
    typedef interface base_type;
    typedef Symbol<Base> visitor_base_type;
  };

  /// A symbol that has a name associated with it.
  class Named {
  private:
    typedef Inherit1::apply<Virtual<Symbol<Base> > >::type interface_base_type;

  public:
    class interface : public interface_base_type { 
    private:
      std::string the_name;

    public:
      interface(const std::string &n)
	: the_name(n) {};

      const std::string &name(void) const {
	return(the_name);
      }
      virtual void accept(mirv::SymbolVisitor &) {
	error("Named::accept called");
      }
    };
    typedef interface base_type;
    typedef Symbol<Base> visitor_base_type;
  };
}

#include <mirv/core/ir/symbol.ii>

#endif
