#ifndef mirv_Core_Builder_Domain_hpp
#define mirv_Core_Builder_Domain_hpp

#include <mirv/Core/Builder/DomainFwd.hpp>

//#include <mirv/Core/Builder/Grammar.hpp>
#include <mirv/Core/Builder/Wrapper.hpp>

namespace mirv {
   namespace Builder {
     /// This defines the domain of all builder expressions.  It
     /// ensures that all operators wrap their proto expressions in
     /// the Wrapper so that customizations apply to all proto
     /// expressions.  In particular, it ensures that members like
     /// else_ and while_ are accessible.
     struct Domain
         : boost::proto::domain<
       boost::proto::pod_generator<Wrapper>
       /*, ConstructGrammar*/>
     {};
   }
}

#endif
