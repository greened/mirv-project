#ifndef mirv_core_builder_builder_hh
#define mirv_core_builder_builder_hh

#include "domain.hh"
#include "expression.hh"
#include "statement.hh"

namespace mirv {
   namespace Builder {
      struct Grammar
            : boost::proto::or<
         StatementGrammar,
         ExpressionGrammar>{};
   }

   template<typename BuildExpr>
   typename Builder::Grammar::template apply<BuildExpr, int, int>::type
   build(const BuildExpr &expr)
   {
      int i = 0; // not used, dummy state and visitor parameter
      return(Builder::Grammar::call(expr, i, i));
   }
}

#endif
