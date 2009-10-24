#ifndef mirv_core_ir_property_hh
#define mirv_core_ir_property_hh

#include <boost/mpl/inherit.hpp>
#include <boost/mpl/transform.hpp>
#include <inherit.hh>

// A Property is a class that allows visitors and other objects that
// query types to only operate on subclasses in which they are
// interested.

namespace mirv {
  // Properties of node types.  Nodes with these properties should
  // inherit from ::type.  Generator is a metafunction class that
  // generates a type appropriate to inherit from.  For example, if
  // Sequence is <Associative, Commutative, Reflexive>, Generator
  // might create Expression<Associative>, Expression<Commutative>
  // and Expression<Reflexive>.  Base is the immediate base class of
  // the expression node.  It may itself inherit from classes further
  // up the tree.
  
  template<typename Generator, typename Base, typename Sequence>
  class Properties {   
    typedef typename lib::InheritScattered<
      typename boost::mpl::transform<Sequence, Generator>::type
      >::type properties_hierarchy_type;
  
public:
  typedef typename boost::mpl::inherit<
  Base, 
  properties_hierarchy_type>::type type;
};
};

#endif
