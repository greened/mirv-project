#ifndef mirv_lib_inherit_hh
#define mirv_lib_inherit_hh

#include <boost/mpl/empty_base.hh>
#include <boost/mpl/fold.hh>
#include <boost/mpl/inherit.hh>

namespace mirv {
  namespace lib {
    template<typename Sequence, typename Node, typename Root = boost::mpl::empty_base>
    struct InheritScattered
  }
}

#endif
