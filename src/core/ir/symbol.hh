#ifndef mirv_core_ir_symbol_hh
#define mirv_core_ir_symbol_hh

#include <mirv/core/ir/base.hh>
#include <mirv/core/ir/node.hh>

namespace mirv {
   template<typename Tag>
   class Symbol : public Tag::base_type {
   public:
     typedef typename Tag::base_type base_type;

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
  };
  class LeafSymbol : public LeafImpl<Symbol<Base> > {
  };
}

#endif
