#ifndef mirv_core_builder_domain_hh
#define mirv_core_builder_domain_hh

#include <mirv/core/builder/grammar.hh>
#include <mirv/core/builder/wrapper.hh>

namespace mirv {
   namespace Builder {
     /// This defines the domain of all builder expressions.  It
     /// ensures that all operators wrap their proto expressions in
     /// the Wrapper so that customizations apply to all proto
     /// expressions.  In particular, it ensures that members like
     /// else_ and while_ are accessible.
     struct Domain
       : boost::proto::domain<boost::proto::pod_generator<Wrapper>,
			      ConstructGrammar>
     {};
   }
}

#endif
