#ifndef MIRVInherit_hh
#define MIRVInherit_hh

// Define inheritance mechanisms for extending the class hierarchy.

#include <mirv/ir/base.hh>

namespace MIRV {
   // The default inheritance mechanism
   template<typename Tag>
   class BaseType {
   public:
      typedef typename Tag::base_type type;
   };

   template<>
   class BaseType<Base> {
   public:
      typedef void type;
   };
}

#endif
