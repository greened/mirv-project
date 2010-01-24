#ifndef mirv_core_ir_Property_hpp
#define mirv_core_ir_Property_hpp

#include <boost/mpl/transform.hpp>
#include <boost/mpl/empty_base.hpp>
#include <mirv/core/ir/Inherit.hpp>
#include <Inherit.hpp>

/// A Property is a class that allows visitors and other objects that
/// query types to only operate on subclasses in which they are
/// interested.

namespace mirv {
  /// Properties of node types.  Nodes with these properties should
  /// inherit from ::type.  Generator is a metafunction class that
  /// generates a type appropriate to inherit from.  For example, if
  /// Sequence is <Associative, Commutative, Reflexive>, Generator
  /// might create Expression<Associative>, Expression<Commutative>
  /// and Expression<Reflexive>.  Base is the immediate base class of
  /// the expression node.  It may itself inherit from classes further
  /// up the tree.
  template<typename Generator, typename Base, typename Sequence,
	   typename InheritOp>
  class Properties {   
    typedef typename lib::InheritScattered<
      typename boost::mpl::transform<Sequence, Generator>::type,
      Virtual<boost::mpl::empty_base>,
      InheritOp
      >::type PropertiesHierarchyType;
  
public:
  typedef typename InheritOp::template apply<
  Base, 
  PropertiesHierarchyType>::type type;
};
};

#endif
