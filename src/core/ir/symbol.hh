#ifndef mirv_core_ir_symbol_hh
#define mirv_core_ir_symbol_hh

#include <mirv/ir/base.hh>
#include <mirv/ir/node.hh>

namespace mirv {
   template<typename Tag>
   class Symbol 
     : public Tag::base_type {
   public:
     typedef typename Tag::base_type base_type;
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
 
