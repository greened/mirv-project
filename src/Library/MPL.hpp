#ifndef mirv_lib_mpl_hh
#define mirv_lib_mpl_hh

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/placeholders.hpp>

namespace mirv {
  namespace lib {
    template<typename Start,
      typename Op,
      typename ...Sequence> 
    struct Fold;

    /// This is a metafunction to perform a fold operation.  It works
    /// similarly to boost::mpl::fold but accepts a variable number of
    /// arguments as the sequence.
    template<typename Start,
      typename Op,
      typename Item,
      typename ...Sequence> 
    struct Fold<Start, Op, Item, Sequence...> 
        : Fold<
      typename boost::mpl::apply<Op, Start, Item>::type,
      Op,
      Sequence...> {};

    template<typename Start, typename Op>
    struct Fold<Start, Op> {
      typedef Start type;
    };
    
    /// Translate a vararg template list to an mpl vector.  This
    /// bridges the semantic gap between C++0x and mpl.
    template<typename ...Items>
    struct AsVector {
      typedef typename Fold<
        boost::mpl::vector<>,
        boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>,
        Items...
        >::type type;
    };
  }
}

#endif
