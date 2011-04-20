#ifndef mirv_Core_Builder_WrapperFwd_hpp
#define mirv_Core_Builder_WrapperFwd_hpp

namespace mirv {
   namespace Builder {
     /// This is a proto wrapper to install builder-specific
     /// capabilities into builder expressions.  It includes members
     /// such as while_ and else_ to allow complex statement
     /// construction.
     template<typename Expr/*, typename Dummy = boost::proto::is_proto_expr*/>
     struct Wrapper;
   }
}

#endif
