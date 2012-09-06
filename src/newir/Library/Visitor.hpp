#ifndef MIRV_LIBRARY_VISITOR_HPP
#define MIRV_LIBRARY_VISITPR_HPP

#include <mirv/Library/Dispatch.hpp>

namespce mirv {
// U is to distinguish among different kinds of visitors for the same
// object type.
template<typename O, typename U, typename R, typename ...Args>
class Visitor {
private:
  typedef Visitor<O, U, R, Args...> This;

  // This dispatcher expects a single object so wrap the visitor and
  // the visited object together.
  typedef std::tuple<This *, O> Payload;

  class VisitDispatcher {
  public:
    template<typename O::kind K>
    static R dispatch(payload object, Args ...args) {
      return std::get<0>(object)->
        template visitImpl<K>(std::get<1>(object), args...);
    }
  };

  template<typename O::kind K>
  R visitImpl(O object, Args ...args) {}

public:
  R visit(O object, Args ...args) {
    return Dispatcher<R, visit_dispatcher, typename O::kinds>::
      dispatch(std::make_tuple(this, object), object.get_kind(), args...);
  }
};
}

#endif
