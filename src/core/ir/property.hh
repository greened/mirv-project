#ifndef MIRVProperty_hh
#define MIRVProperty_hh

#include <boost/mpl/apply.hpp>
#include <boost/mpl/placeholders.hpp>

// A Property is a class that allows visitors and other objects that
// query types to only operate on subclasses in which they are
// interested.

namespace MIRV {

   // Properties of node types.  Nodes with these properties should
   // inherit from ::type.  Generator is a metafunction class that
   // generates a type appropriate to inherit from.  For example, if
   // Sequence is <Associative, Commutative, Reflexive>, Generator
   // might create Expression<Associative>, Expression<Commutative>
   // and Expression<Reflexive>.  Root is the topmost class in the
   // inheritance subtree.  It may itself inherit from classes further
   // up the tree.

   template<typename Generator, typename Root, typename Sequence>
   class Properties {
   public:
      typedef typename boost::mpl::inherit_linearly<
      Sequence,
      boost::mpl::apply2<Generator, boost::mpl::_2, boost::mpl::_1>,
      Root
      >::type type;
   };
};

#endif
