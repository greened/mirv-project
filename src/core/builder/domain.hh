#ifndef mirv_core_builder_domain_hh
#define mirv_core_builder_domain_hh

#include <mirv/core/builder/grammar.hh>
#include <mirv/core/builder/wrapper.hh>

namespace mirv {
   namespace Builder {
      struct Domain
	: boost::proto::domain<boost::proto::pod_generator<Wrapper>,
			       ConstructGrammar>
      {};
   }
}

#endif
