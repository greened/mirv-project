#ifndef mirv_lib_inherit_hh
#define mirv_lib_inherit_hh

#include <boost/mpl/inherit.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/quote.hpp>

namespace mirv {
  namespace lib {
    template<typename Sequence,
	     typename Base,
	     typename Inherit = boost::mpl::quote2<boost::mpl::inherit2> >
    struct InheritScattered : 
      public boost::mpl::fold<
      Sequence, 
      Base, 
      Inherit
      > {};
  }
}

#endif
