#ifndef MIRVDebug_hh
#define MIRVDebug_hh

#include <iostream>

#define check_invariant(Check, Msg) \
   if (!(Check)) {                              \
      std::cerr << "PANIC: " << Msg << "\n";    \
   }

#endif
