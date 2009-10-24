#ifndef mirv_core_ir_base_hh
#define mirv_core_ir_base_hh

#include <boost/mpl/vector.hpp>

namespace mirv {
   // Signifies the root of some IR class subtree,
   // e.g. Expression<Base>, Statement<Base>, etc.
   struct Base {
     typedef Base base_type;
     typedef void child_type;
     typedef void visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };
}

#endif
