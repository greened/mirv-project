#ifndef mirv_core_ir_base_hh
#define mirv_core_ir_base_hh

namespace MIRV {
   // Signifies the root of some IR class subtree,
   // e.g. Expression<Base>, Statement<Base>, etc.
   struct Base {
     typedef Base base_type;
     typedef void child_type;
   };
}

#endif
