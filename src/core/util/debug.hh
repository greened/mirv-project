#ifndef mirv_core_util_debug_hh
#define mirv_core_util_debug_hh

#include <iostream>
#include <cstdlib>

/// Assert that Check is true and abort with Msg if it is not.
#define check_invariant(Check, Msg) \
   if (!(Check)) {                              \
      error(Msg);                               \
   }

// TODO: Make a proper singleton
namespace mirv {
  /// Abort with a message.
  inline void error(const std::string &msg) {
    std::cerr << "ERROR: " << msg << "\n";
    abort();
  }

  /// This class manages debug state.  It holds various flags and
  /// information that may be queried to determine what kind of debug
  /// checks and/or output to perform.
  class DebugManager {
  private:
    static DebugManager *instance;

    /// Is debugging active?
    bool active;

    enum Flags {
      /// Check all casts for correctness.
      SafeCast = 0x1
    };

    /// What should be debugged?
    unsigned flags;

  public:
    /// Return whether debugging is active.
    bool IsActive(void) { return active; }
    /// Return whether any features implied by flag is active.
    bool Feature(Flags flag) { return flags & flag; }

    /// Get the instance of the DebugManager.
    static DebugManager &Instance(void) {
      return *(instance ? instance : instance = new DebugManager);
    }
  };
}

#endif
