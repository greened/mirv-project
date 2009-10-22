#ifndef mirv_core_ir_semantics_hh
#define mirv_core_ir_semantics_hh

#include <inherit.hh>

// Semantics of known node types.  Additional semantics can be
// added via plugins.  It's not clear how this should work yet.
// Should additional semantics be built from these or can 
// entirely new concepts be introduced?  Probably the latter,
// but how?

namespace mirv {

  template<typename Tag>
  class Semantic;

  template<>
  class Semantic<Base> {};

  // The default inheritance mechanism
  //template<typename Tag>
  //class BaseType<Semantic<Tag> > {
  //  typedef BaseSemantic type;
  //};
}

#endif
