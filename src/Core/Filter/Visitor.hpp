#ifndef mirv_Core_Filter_Visitor_hpp
#define mirv_Core_Filter_Visitor_hpp

#include <experimental/type_traits>

namespace mirv {
  namespace detail {
    // Determine whether visitor V can visit a base type B.
    template<typename V, typename B>
    using Visitable = decltype(std::declval<V>().visit(std::declval<B &>()));

    template<typename V, typename B>
    using SupportsVisit = std::experimental::is_detected<Visitable, V, B>;

    // Visit an object as its various base classes.
    template<typename VisitorType, typename Derived, typename Base,
             std::enable_if_t<SupportsVisit<VisitorType, Base>::value> * = nullptr>
    bool doVisitIfVisitable(Derived &D, VisitorType &V) {
      Base &B = D;
      V.visit(B);
      return true;
    }

    template<typename VisitorType, typename Derived, typename Base,
             std::enable_if_t<!SupportsVisit<VisitorType, Base>::value> * = nullptr>
    bool doVisitIfVisitable(Derived &D, VisitorType &V) {
      return false;
    }

    // template<typename VisitorType, typename Derived, typename Base>
    // void doVisitHelper(Derived &D, VisitorType &V) {
    //   doVisitIfVisitable<VisitorType, Derived, Base>(D, V);
    // }

    template<typename VisitorType, typename Derived, typename ...Bases>
    struct BaseVisitor {};

    template<typename VisitorType, typename Derived>
    struct BaseVisitor<VisitorType, Derived> {
      static bool doVisitHelper(Derived &D, VisitorType &V) {
        return false;
      }
    };

    template<typename VisitorType, typename Derived, typename Base>
    struct BaseVisitor<VisitorType, Derived, Base> {
      static bool doVisitHelper(Derived &D, VisitorType &V) {
        if (!doVisitIfVisitable<VisitorType, Derived, Base>(D, V)) {
          // Try base classes of Base.
          return BaseVisitor<VisitorType, Base,
                             typename Base::VisitorBaseTypes>::
            doVisitUnpack(D, V);
        }
        return true;
      }
    };

    template<typename VisitorType, typename Derived, typename Base,
             typename ...Bases>
    struct BaseVisitor<VisitorType, Derived, Base, Bases...> {
      static bool doVisitHelper(Derived &D, VisitorType &V) {
        if (!doVisitIfVisitable<VisitorType, Derived, Base>(D, V)) {
          // Try base classes of Base.
          if (!BaseVisitor<VisitorType, Base, typename Base::VisitorBaseTypes>::
              doVisitUnpack(D, V)) {
            // Try the next base in Derived.
            return BaseVisitor<VisitorType, Derived, Bases...>::
              doVisitHelper(D, V);
          }
        }
        return true;
      }
    };

    template<typename VisitorType, typename Derived, typename ...Bases>
    struct BaseVisitor<VisitorType, Derived, TypeList<Bases...>> {
      static bool doVisitUnpack(Derived &D, VisitorType &V) {
        return BaseVisitor<VisitorType, Derived, Bases...>::doVisitHelper(D, V);
      }
    };
  }

  template<typename VisitorType, typename Derived>
  void doVisit(VisitorType &V, Derived &D) {
    detail::BaseVisitor<VisitorType, Derived,
                        typename Derived::VisitorBaseTypes>::
      doVisitUnpack(D, V);
  }
}

#endif
