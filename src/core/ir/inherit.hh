#ifndef mirv_core_ir_inherit_hh
#define mirv_core_ir_inherit_hh

// Define inheritance mechanisms for extending the class hierarchy.

#include <mirv/ir/base.hh>
#include <boost/mpl/empty_base.hpp>

namespace mirv {
   // The default inheritance mechanism
   template<typename Tag>
   class BaseType {
   public:
      typedef typename Tag::base_type type;
   };

   template<>
   class BaseType<Base> {
   public:
     typedef boost::mpl::empty_base type;
   };

  // Inherit virtually
  template<typename Base>
  struct Virtual : public virtual Base {};
}

#endif
