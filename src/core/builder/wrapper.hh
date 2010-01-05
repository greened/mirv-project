#ifndef mirv_core_builder_wrapper_hh
#define mirv_core_builder_wrapper_hh

#include <mirv/core/builder/domain_fwd.hh>

namespace mirv {
   namespace Builder {
     namespace keyword {
       struct if_;
       struct else_;
       struct do_;
       struct while_;
     };

     template<typename Expr/*, typename Dummy = boost::proto::is_proto_expr*/>
     struct Wrapper {
       BOOST_PROTO_EXTENDS(Expr, Wrapper<Expr>, Domain)

       // Use BOOST_PROTO_EXTENDS_MEMBERS() to define "virtual"
       // data members that all expressions in the builder
       // domain will have. They can be used to create expressions
       // like "if_(x)[y].else_[z]" and "do_[y].while_(z)".
       BOOST_PROTO_EXTENDS_MEMBERS(
				   ((keyword::else_,   else_))
				   ((keyword::while_,  while_))
				   )
     };
   }
}

#endif
