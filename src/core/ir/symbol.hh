#ifndef mirv_core_ir_symbol_hh
#define mirv_core_ir_symbol_hh

#include <mirv/core/ir/base.hh>
#include <mirv/core/ir/node.hh>
#include <mirv/core/ir/type_fwd.hh>

namespace mirv {
  struct SymbolVisitor;

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

   template<>
   class Symbol<Base> : public Node<Base> { 
   public:
     virtual void accept(SymbolVisitor &V);
   };

  class InnerSymbol : public InnerImpl<Symbol<Base>, VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> {
  public:
    typedef Symbol<Base> visitor_base_type;
    virtual void accept(SymbolVisitor &V);
  };

  class LeafSymbol : public LeafImpl<VisitedInherit1<SymbolVisitor>::apply<Virtual<Symbol<Base> > >::type> {
  public:
    typedef Symbol<Base> visitor_base_type;
    virtual void accept(SymbolVisitor &V);
  };

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
