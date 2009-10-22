#ifndef mirv_core_ir_symbol_hh
#define mirv_core_ir_symbol_hh

#include <mirv/ir/base.hh>
#include <mirv/ir/node.hh>

namespace mirv {
   template<
      typename Tag,
      typename Base = typename BaseType<Tag>::type>
   class Symbol 
         : public Base {
   public:
      typedef Base base_type;
   };

   typedef Symbol<Base> BaseSymbol;
  class InnerSymbol : public InnerImpl<BaseSymbol> {
  };
  class LeafSymbol : public LeafImpl<BaseSymbol> {
  };
}

#endif
 
