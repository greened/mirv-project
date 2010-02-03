#ifndef mirv_Core_Builder_Wrapper_hpp
#define mirv_Core_Builder_Wrapper_hpp

#include <mirv/Core/Builder/DomainFwd.hpp>

namespace mirv {
   namespace Builder {
     namespace keyword {
       struct if_;
       struct else_;
       struct do_;
       struct while_;
       struct type;
     };

     /// This is a proto wrapper to install builder-specific
     /// capabilities into builder expressions.  It includes members
     /// such as while_ and else_ to allow complex statement
     /// construction.
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
				   ((keyword::type,    type))
				   )
     };
   }
}

#endif
