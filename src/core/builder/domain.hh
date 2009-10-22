#ifndef mirv_core_builder_domain_hh
#define mirv_core_builder_domain_hh

namespace mirv {
   namespace Builder {
      struct Domain;

      template<typename Expr>
      struct Expression
            : proto::extends<Expr, Expression<Expr>, Domain> {
         typedef proto::extends<Expr, Expression<Expr>, Domain>
         base_type;

         Expression(Expr const & expr = Expr())
               : base_type( expr )
         {}
      };

      struct Domain
            : proto::domain< proto::generator< Expression > >
      {};
   }
}

#endif
