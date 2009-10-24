#ifndef mirv_lib_inherit_hh
#define mirv_lib_inherit_hh

#include <boost/mpl/inherit.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/quote.hpp>

namespace mirv {
  namespace lib {
    namespace detail {
    }

    template<typename Sequence, typename Root = boost::mpl::empty_base>
    struct InheritScattered : 
      public boost::mpl::fold<
      Sequence, 
      Root, 
      boost::mpl::quote2<boost::mpl::inherit2>
      > {};
  }
}

#endif
