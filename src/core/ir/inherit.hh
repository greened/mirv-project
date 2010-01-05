#ifndef mirv_core_ir_inherit_hh
#define mirv_core_ir_inherit_hh

// Define inheritance mechanisms for extending the class hierarchy.

#include <mirv/core/util/debug.hh>

namespace mirv {
  // Inherit virtually.  Use for specialization.
  template<typename Base>
  class Virtual : public virtual Base {
  private:
    // Don't ever allow this to be constructed.
    Virtual<Base>(void);
    Virtual<Base>(const Virtual<Base> &);
  };

  namespace detail {
    template<typename T1>
    class Type1 : public T1 {
    public:
      Type1(void) {}

      template<typename A1>
      Type1(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      Type1(A1 a1, A2 a2) : T1(a1, a2) {}
    };

    template<typename T1>
    class Type1<Virtual<T1> > : public virtual T1 {
    public:
      Type1(void) {}

      template<typename A1>
      Type1(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      Type1(A1 a1, A2 a2) : T1(a1, a2) {}
    };

    template<typename T1, typename T2>
    class Type2 : public T1, public T2 {
    public:
      Type2(void) {} 

     template<typename A1>
      Type2(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      Type2(A1 a1, A2 a2) : T1(a1, a2) {}

      template<typename A1, typename A2, typename A3>
      Type2(A1 a1, A2 a2, A3 a3) : T1(a1, a2, a3) {}
    };

    template<typename T1, typename T2>
    class Type2<T1, Virtual<T2> > :
      public T1, public virtual T2 {
    public:
      Type2(void) {}

      template<typename A1>
      Type2(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      Type2(A1 a1, A2 a2) : T1(a1, a2) {}

      template<typename A1, typename A2, typename A3>
      Type2(A1 a1, A2 a2, A3 a3) : T1(a1, a2, a3) {}
    };

    template<typename T1, typename T2>
    class Type2<Virtual<T1>, T2> :
      public virtual T1, public T2 {
    public:
      Type2(void) {}

      template<typename A1>
      Type2(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      Type2(A1 a1, A2 a2) : T1(a1, a2) {}

      template<typename A1, typename A2, typename A3>
      Type2(A1 a1, A2 a2, A3 a3) : T1(a1, a2, a3) {}
    };

    // Types that accept visitors.
    template<typename Visitor, typename T1>
    class VisitedType1 : public T1 {
    public:
      VisitedType1(void) {}

      template<typename A1>
      VisitedType1(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      VisitedType1(A1 a1, A2 a2) : T1(a1, a2) {}

      void accept(Visitor &V) {
	error("Inherit::accept called");
      }
    };

    template<typename Visitor, typename T1>
    class VisitedType1<Visitor, Virtual<T1> > : public virtual T1 {
    public:
      VisitedType1(void) {}

      template<typename A1>
      VisitedType1(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      VisitedType1(A1 a1, A2 a2) : T1(a1, a2) {}

      void accept(Visitor &V) {
	error("Inherit::accept called");
      }
    };

    template<typename Visitor, typename T1, typename T2>
    class VisitedType2 : public T1, public T2 {
    public:
      VisitedType2(void) {} 

     template<typename A1>
      VisitedType2(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      VisitedType2(A1 a1, A2 a2) : T1(a1, a2) {}

      template<typename A1, typename A2, typename A3>
      VisitedType2(A1 a1, A2 a2, A3 a3) : T1(a1, a2, a3) {}

      void accept(Visitor &V) {
	error("Inherit::accept called");
      }
    };

    template<typename Visitor, typename T1, typename T2>
    class VisitedType2<Visitor, T1, Virtual<T2> > :
      public T1, public virtual T2 {
    public:
      VisitedType2(void) {}

      template<typename A1>
      VisitedType2(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      VisitedType2(A1 a1, A2 a2) : T1(a1, a2) {}

      template<typename A1, typename A2, typename A3>
      VisitedType2(A1 a1, A2 a2, A3 a3) : T1(a1, a2, a3) {}

      void accept(Visitor &V) {
	error("Inherit::accept called");
      }
    };

    template<typename Visitor, typename T1, typename T2>
    class VisitedType2<Visitor, Virtual<T1>, T2> :
      public virtual T1, public T2 {
    public:
      VisitedType2(void) {}

      template<typename A1>
      VisitedType2(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      VisitedType2(A1 a1, A2 a2) : T1(a1, a2) {}

      template<typename A1, typename A2, typename A3>
      VisitedType2(A1 a1, A2 a2, A3 a3) : T1(a1, a2, a3) {}

      void accept(Visitor &V) {
	error("Inherit::accept called");
      }
    };
  }

  class Inherit2 {
  public:
    template<typename T1, typename T2>
    struct apply {
      typedef detail::Type2<T1, T2> type;
    };
  };

  class Inherit1 {
  public:
    template<typename T1>
    struct apply {
      typedef detail::Type1<T1> type;
    };
  };

  // Inhertance for visited types.
  template<typename Visitor>
  class VisitedInherit2 {
  public:
    template<typename T1, typename T2>
    struct apply {
      typedef detail::VisitedType2<Visitor, T1, T2> type;
    };
  };

  template<typename Visitor>
  class VisitedInherit1 {
  public:
    template<typename T1>
    struct apply {
      typedef detail::VisitedType1<Visitor, T1> type;
    };
  };
 }

#endif
