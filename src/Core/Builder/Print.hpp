#ifndef mirv_Core_Builder_Print_hpp
#define mirv_Core_Builder_Print_hpp

#include <boost/proto/proto.hpp>

namespace mirv {
  namespace Builder {
    /// This is a utility to check if a builder fragment matches the
    /// builder grammar.  It is mainly used for debugging.
    template<typename Grammar, typename Expr>
    void
    printMatch(const Expr &expr) {
      bool result = boost::proto::matches<typename boost::proto::result_of::as_expr<Expr, Domain>::type, Grammar>::value;
      std::cout << "Match for " << typeid(expr).name() << ": " << result
        << '\n';
    }
  }
}

#endif
