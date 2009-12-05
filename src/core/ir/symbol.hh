#ifndef mirv_core_ir_symbol_hh
#define mirv_core_ir_symbol_hh

#include <mirv/core/ir/base.hh>
#include <mirv/core/ir/node.hh>

namespace mirv {
   template<typename Tag>
   class Symbol 
     : public Tag::base_type {
   public:
     Symbol(const typename Tag::base_type &base) :
       Tag::base_type(base) {}
     typedef typename Tag::base_type base_type;

     template<typename A1>
     static typename ptr<Symbol<Tag> >::type
     make(A1 a1) {
       return new Symbol<Tag>(typename Tag::base_type(a1));
     }

     template<typename A1, typename A2>
     static typename ptr<Symbol<Tag> >::type
     make(A1 a1, A2 a2) {
       return new Symbol<Tag>(typename Tag::base_type(a1, a2));
     }
   };

   template<>
   class Symbol<Base> : public Node<Base> { 
   public:
   };

  class InnerSymbol : public InnerImpl<Symbol<Base>, Virtual<Symbol<Base> > > {
  };
  class LeafSymbol : public LeafImpl<Virtual<Symbol<Base> > > {
  };
}

#endif
