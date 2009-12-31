#ifndef mirv_core_builder_domain_hh
#define mirv_core_builder_domain_hh

#include <mirv/core/builder/grammar.hh>

namespace mirv {
   namespace Builder {
      struct Domain;

     template<typename Expr, typename Dummy = boost::proto::is_proto_expr>
     struct Expression {
       BOOST_PROTO_EXTENDS(Expr, Expression<Expr>, Domain)
     };

      struct Domain
	: boost::proto::domain<boost::proto::pod_generator<Expression>,
			       ConstructGrammar>
      {};
   }
}

#endif
