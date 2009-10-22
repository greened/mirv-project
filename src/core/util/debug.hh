#ifndef mirv_core_util_debug_hh
#define mirv_core_util_debug_hh

#include <iostream>

#define check_invariant(Check, Msg) \
   if (!(Check)) {                              \
      std::cerr << "PANIC: " << Msg << "\n";    \
      abort();                                  \
   }

// TODO: Make a proper singleton
namespace mirv {
  class DebugManager {
  private:
    static DebugManager *instance;

    bool active;

    enum Flags {
      SafeCast = 0x1
    };

    unsigned flags;

  public:
    bool IsActive(void) { return active; }
    bool Feature(Flags flag) { return flags & flag; }

    static DebugManager &Instance(void) {
      return *(instance ? instance : instance = new DebugManager);
    }
  };
}

#endif
