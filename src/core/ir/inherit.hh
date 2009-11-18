#ifndef mirv_core_ir_inherit_hh
#define mirv_core_ir_inherit_hh

// Define inheritance mechanisms for extending the class hierarchy.

#include <mirv/util/debug.hh>

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
    template<typename Visitor, typename T1, typename T2>
    class Type : public T1, public T2 {
    public:
      void accept(Visitor &V) {
	error("Inherit::accept called");
      }
    };

    template<typename Visitor, typename T1, typename T2>
    class Type<Visitor, T1, Virtual<T2> > :
      public T1, public virtual T2 {
    public:
      void accept(Visitor &V) {
	error("Inherit::accept called");
      }
    };
  }

  template<typename Visitor>
  class Inherit {
  public:
    template<typename T1, typename T2>
    struct apply {
      typedef detail::Type<Visitor, T1, T2> type;
    };
  };
 }

#endif
