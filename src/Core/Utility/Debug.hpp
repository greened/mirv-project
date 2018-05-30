#ifndef mirv_Core_Utility_Debug_hpp
#define mirv_Core_Utility_Debug_hpp

#include <mirv/Core/Memory/Heap.hpp>

#include <iostream>
#include <cstdlib>

/// Assert that Check is true and abort with Msg if it is not.
#define checkInvariant(Check, Msg) \
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

  /// Abort with a message.
  inline void unreachable(const std::string &msg) {
    std::cerr << "UNREACHABLE: " << msg << "\n";
    abort();
  }

  /// This class manages debug state.  It holds various flags and
  /// information that may be queried to determine what kind of debug
  /// checks and/or output to perform.
  class DebugManager {
  private:
    static DebugManager *inst;

    /// Is debugging active?
    bool active;

  public:
    enum Flags {
      /// Check all casts for correctness.
      SafeCast = 0x1
    };

  private:
    /// What should be debugged?
    unsigned flags;

  public:
    /// Return whether debugging is active.
    bool isActive(void) { return active; }
    /// Return whether any features implied by flag is active.
    bool feature(Flags flag) { return flags & flag; }

    /// Get the instance of the DebugManager.
    static DebugManager &instance(void) {
      return *(inst ? inst : inst = new DebugManager);
    }
  };

  class Type;
  class Control;
  class ValueProducer;

  void dump(ptr<const Type> sym);
  void dump(ptr<Control> statement);
  void dump(ptr<ValueProducer> expression);
}

#endif
