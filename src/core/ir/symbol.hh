#ifndef mirv_core_ir_symbol_hh
#define mirv_core_ir_symbol_hh

#include <mirv/core/ir/base.hh>
#include <mirv/core/ir/node.hh>
#include <mirv/core/ir/type_fwd.hh>

namespace mirv {
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
   };

   template<>
   class Symbol<Base> : public Node<Base> { 
   public:
   };

  class InnerSymbol : public InnerImpl<Symbol<Base>, Symbol<Base> > {
  public:
    typedef Symbol<Base> visitor_base_type;
  };
  class LeafSymbol : public LeafImpl<Symbol<Base> > {
  public:
    typedef Symbol<Base> visitor_base_type;
  };

  class Typed {
  private:
    typedef Inherit1::apply<Virtual<LeafSymbol> >::type interface_base_type;

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
    };
    typedef interface base_type;
    typedef LeafSymbol visitor_base_type;
  };

  class Named {
  private:
    typedef Inherit1::apply<Virtual<LeafSymbol> >::type interface_base_type;

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
    };
    typedef interface base_type;
    typedef LeafSymbol visitor_base_type;
  };
}

#endif
