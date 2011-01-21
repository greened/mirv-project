#ifndef mirv_Core_IR_Inherit_hpp
#define mirv_Core_IR_Inherit_hpp

// Define inheritance mechanisms for extending the class hierarchy.

#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Utility/Debug.hpp>

namespace mirv {
  /// This is a utility class to specify virtual inheritance.  It is
  /// used by generator clients to specify the base of the generator
  /// result should be inherited virtually.
  template<typename Base>
  class Virtual : public virtual Base {
  public:
    Virtual<Base>(void) : Base() {}
    template<typename ...Arg>
    Virtual<Base>(const Arg &...arg) : Base(arg...) {};
  };
}

#endif
