#ifndef mirv_Library_Singleton_hpp
#define mirv_Library_Singleton_hpp

// This is a very basic singleton, not useful for much beyond simple
// concepts.  This should be replaced by a more robust implementation.

#include <memory>

namespace mirv {
  template<typename Type>
  class Singleton {
  public:
    using Handle = std::unique_ptr<Type>;

  private:
    static Handle inst;

  protected:
    Singleton(void) {}

  public:
    static Type &instance(void) {
      if (!inst) {
        // Can't easily use make_unique because Type's constructor
        // should be private.
        inst.reset(new Type);
      }
      return *inst;
    }
  };

  template<typename Type>
  typename mirv::Singleton<Type>::Handle mirv::Singleton<Type>::inst;
}

#endif
