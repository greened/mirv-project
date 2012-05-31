#ifndef mirv_Library_Singleton_hpp
#define mirv_Library_Singleton_hpp

// This is a very basic singleton, not useful for much beyond simple
// concepts.  This should be replaced by a more robust implementation.

#include <memory>

namespace mirv {
  namespace lib {
    template<typename Type>
    class Singleton {
    private:
      static std::shared_ptr<Type> inst;

    protected:
      Singleton(void) {}

    public:
      static Type &instance(void) {
        if (!inst) {
          inst = std::shared_ptr<Type>(new Type);
        }
        return *inst;
      }
    };
  }
}

#endif
