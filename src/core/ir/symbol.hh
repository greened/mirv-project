#ifndef MIRVSymbol_hh
#define MIRVSymbol_hh

namespace MIRV {
   template<
      typename Tag,
      typename Base = typename BaseType<Tag>::type>
   class Symbol 
         : public Base {
   public:
      typedef Base base_type;
   };

   typedef Symbol<Base> BaseSymbol;
   typedef Inner<BaseSymbol> InnerSymbol;
   typedef Leaf<BaseSymbol> LeafSymbol;
}

#endif
 
