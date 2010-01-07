#ifndef mirv_core_builder_builder_hh
#define mirv_core_builder_builder_hh

// Order is important here.  In particular, domain.hh must be included
// after grammar.hh.
#include <mirv/core/builder/expression.hh>
#include <mirv/core/builder/statement.hh>
#include <mirv/core/builder/grammar.hh>
#include <mirv/core/builder/domain.hh>
#include <mirv/core/builder/wrapper.hh>
#include <mirv/core/builder/make.hh>
#include <mirv/core/builder/translate.hh>

namespace mirv {
  namespace Builder {
#if 0
      // An if_ "operator"
     template<typename Expr>
     typename boost::proto::result_of::make_expr<
       boost::proto::tag::function,
       Domain,
       keyword::if_,
       Expr>::type
     if_(Expr const &expr)
     {
       return boost::proto::make_expr<boost::proto::tag::function, Domain>(
         keyword::if_(),
	 expr);
     }

    // A while_ "operator"
     template<typename Expr>
     typename boost::proto::result_of::make_expr<
       boost::proto::tag::function,
       Domain,
       keyword::while_,
       Expr>::type
     while_(Expr const &expr)
     {
       return boost::proto::make_expr<boost::proto::tag::function, Domain>(
         keyword::while_(),
	 expr);
     }
#endif

    // An if_ "operator."
    namespace {
       const IfTerminal if_ = {{}};
    }

    // A while_ "operator."
    namespace {
       const WhileTerminal while_ = {{}};
    }

    // A do_ "operator."
    namespace {
       const DoTerminal do_ = {{}};
    }
  }
}

#endif
