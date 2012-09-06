#ifndef MIRV_LIBRARY_DISPATCH_HPP
#define MIRV_LIBRARY_DISPATCH_HPP

namespace mirv {
  /// This is a collection of node kinds.  We use it to categorize
  /// various kinds of objects and dispatch to the correct routine.
  template<typename T, typename T::kind ...I>
  class KindTuple {};

  /// This is the "virtual function" dispatch mechanism.  It
  /// statically generates a lookup table of routines, one for each
  /// kind.
  template<typename R, typename D, typename T>
  class Dispatcher {};

  /// This is the actual dispatch implementation.
  template<typename R, typename D, typename O, typename O::kind ...I>
  class Dispatcher<R, D, KindTuple<O, I...>> {
  private:
    // Call the "virtual function override" via the provided class-level
    // dispatcher.
    template<typename P, typename O::kind K, typename ...Args>
      static R dispatchHandler(P payload, Args ...args) {
      return D::template dispatch<K>(payload, args...);
    }

  public:
    // dispatcher is an object that knows which routine in N to call.
    template<typename P, typename ...Args>
      static R dispatch(P payload, typename O::kind kind, Args ...args) {
      using F = R(P, Args...);

      F *table[] = {
        dispatcHandler<P, I, Args...>
        ...
      };

      return table[kind](payload, args...);
    }
  };
}

#endif
