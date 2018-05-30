#ifndef mirv_Core_Memory_Heap_hpp
#define mirv_Core_Memory_Heap_hpp

#include <cstddef>
#include <type_traits>

namespace mirv {
  // This exists primarily for overloading operators to allow IR
  // construction vi expression templates.  It is not legal to
  // overload operators on bare pointer types so we require a wrapper.
  template<typename T>
  class pointer {
    T *P;

    template<typename O>
    using EnableConstructor =
      typename std::enable_if<std::is_convertible<O *, T *>::value>::type;

  public:
    typedef T PointeeType;

    pointer(void) = default;
    pointer(const pointer<T> &) = default;
    pointer(T *p) : P(p) {}

    template<typename O, typename = EnableConstructor<O>>
    pointer(pointer<O> Other) : P(&*Other) {}
    template<typename O, typename = EnableConstructor<O>>
    pointer(O *p) : P(p) {}

    void destroy(void) {
      delete P;
    }

    void reset(void) {
      P = nullptr;
    }

    operator bool(void) const {
      return P != nullptr;
    }

    T &operator*(void) {
      return *P;
    }

    const T &operator*(void) const {
      return *P;
    }

    T *operator->(void) {
      return P;
    }

    const T *operator->(void) const {
      return P;
    }

    template<typename O>
    bool operator==(pointer<O> Other) const {
      return P == &*Other;
    }
    bool operator==(std::nullptr_t Other) const {
      return P == Other;
    }

    template<typename O>
    bool operator!=(pointer<O> Other) const {
      return !(*this == Other);
    }
    bool operator!=(std::nullptr_t Other) const {
      return P != Other;
    }
  };

  template<typename Type> using ptr = pointer<Type>;
  template<typename Type> using weak_ptr = pointer<Type>;

  template<typename T>
  ptr<T> getHandle(T *p) {
    return ptr<T>(p);
  }
}

namespace std {
  template<typename T>
  struct remove_pointer<mirv::pointer<T>> {
    typedef T type;
  };

  template<typename T>
  mirv::pointer<T> const_pointer_cast(mirv::pointer<const T> Pointer) {
    return getHandle(const_cast<T *>(&*Pointer));
  }
}

#endif
