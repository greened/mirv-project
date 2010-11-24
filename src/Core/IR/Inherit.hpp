#ifndef mirv_Core_IR_Inherit_hpp
#define mirv_Core_IR_Inherit_hpp

// Define inheritance mechanisms for extending the class hierarchy.

#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/Memory/Heap.hpp>
#include <mirv/Core/Utility/Debug.hpp>

namespace mirv {
  /// This is a utility class to specify virtual inheritance.
  /// Specializations of low-level utiltiy classes use this to
  /// determine how to inherit their template arguments.
  template<typename Base>
  class Virtual : public virtual Base {
  private:
    // Don't ever allow this to be constructed.
    Virtual<Base>(void);
    Virtual<Base>(const Virtual<Base> &);
  };

  namespace detail {
    /// This is a low-level class to inherit from a single type.
    template<typename T1>
    class Type1 : public T1 {
    public:
      Type1(void) {}

      template<typename A1>
      Type1(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      Type1(A1 a1, A2 a2) : T1(a1, a2) {}
    };

    /// This is a specialization to inherit virtually from a single
    /// type.
    template<typename T1>
    class Type1<Virtual<T1> > : public virtual T1 {
    public:
      Type1(void) {}

      template<typename A1>
      Type1(A1 a1) : T1(a1) {}

      template<typename A1, typename A2>
      Type1(A1 a1, A2 a2) : T1(a1, a2) {}
    };

    /// This is a low-level class to inherit from two types.
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

    /// This is a specialzation to inherit virtually from one of two
    /// types.
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

    /// This is a specialzation to inherit virtually from one of two
    /// types.
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

    /// This is a low-level type to inherit from a single type and
    /// provide a dummy implementation of the visitor accept function
    /// to avoid issues with multiple inheritance and ambiguous
    /// virtual function overrides.
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

    /// This is a specilaization to inherit virtually from one type.
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

    /// This is a low-level type to inherit from two types and provide
    /// a dummy implementation of the visitor accept function to avoid
    /// issues with multiple inheritance and ambiguous virtual
    /// function overrides.
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

      //ptr<Node<Base>>::type getSharedHandle(void) {
      //  error("Inherit::getSharedHandle called");
      //  return ptr<Node<Base>>::type();
      //}

      void accept(Visitor &V) {
	error("Inherit::accept called");
      }
    };

    /// This is a specialization to inherit virtually from one of two
    /// types.
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

      // ptr<Node<Base>>::type getSharedHandle(void) {
      //   error("Inherit::getSharedHandle called");
      //   return ptr<Node<Base>>::type();
      // }

      void accept(Visitor &V) {
	error("Inherit::accept called");
      }
    };

    /// This is a specialization to inherit virtually from one of two
    /// types.
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

      //ptr<Node<Base>>::type getSharedHandle(void) {
      //  error("Inherit::getSharedHandle called");
      //  return ptr<Node<Base>>::type();
      //}

      void accept(Visitor &V) {
	error("Inherit::accept called");
      }
    };
  }

  /// This is a metafunction class to inherit from two types.
  class Inherit2 {
  public:
    template<typename T1, typename T2>
    struct apply {
      typedef detail::Type2<T1, T2> type;
    };
  };

  /// This is a metafunction class to inherit from one type.
  class Inherit1 {
  public:
    template<typename T1>
    struct apply {
      typedef detail::Type1<T1> type;
    };
  };

  /// This is a metafunction class to inherit from two types and
  /// provide a dummy override for the visitor accept function in
  /// order to avoid issues with multiple inheritance and ambiguous
  /// virtual function overrides.
  template<typename Visitor>
  class VisitedInherit2 {
  public:
    template<typename T1, typename T2>
    struct apply {
      typedef detail::VisitedType2<Visitor, T1, T2> type;
    };
  };

  /// This is a metafunction class to inherit a single type and
  /// provide a dummy override for the visitor accept function in
  /// order to avoid issues with multiple inheritance and ambiguous
  /// virtual function overrides.
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
