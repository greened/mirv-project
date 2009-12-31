#ifndef mirv_core_builder_statement_hh
#define mirv_core_builder_statement_hh

#include <boost/proto/proto.hpp>

namespace mirv {
   namespace Builder {
      struct ConstructExpressionGrammar;
      struct ConstructStatementGrammar;

     typedef boost::proto::assign<ConstructExpressionGrammar, ConstructExpressionGrammar> Assign;

      struct IfTag {};
      struct Else {};
      struct While {};
      struct Do {};
#if 0
      // An if_ "operator"
     typedef boost::proto::terminal<IfTag>::type If;
     If const if_ = {{}};
#endif
#if 0
      BOOST_PROTO_DEFINE_FUNCTION_TEMPLATE(
         1
         , if_
         , MIRV::Builder::Domain
         , (MIRV::Builder::If)
         , BOOST_PP_SEQ_NIL
         );
#endif
#if 0
      // Here is the grammar for if_ statements
      // matches if_(e1)[e2]
      struct IfGrammar
            : boost::proto::subscript<
                 boost::proto::function<
		   //                    boost::proto::unary_expr<
                       If,
		   ConstructExpressionGrammar
		   //                    >,
                 >,
                 ConstructStatementGrammar
              > {};
#endif
#if 0
      // An else_ "operator"
      template<typename Expr>
      struct Expression<
         Expr
         , typename enable_if<proto::matches<Expr, IfGrammar> >::type
      >
      : proto::extends<Expr, Expression<Expr>, Domain>
      {
         Expression()
               : else_(proto::make_expr<Else>(*this)) {};

         proto::result_of::make_expr<
            Else, // else is now an "operator"
            Expression<Expr> const
         >::type const else_;
      };

      BOOST_PROTO_DEFINE_FUNCTION_TEMPLATE(
         1
         , while_
         , MIRV::Builder::Domain
         , (MIRV::Builder::While)
         , BOOST_PP_SEQ_NIL
         );
#endif

#if 0
      // matches if_(e1)[e2].else_[e3]
      struct IfElseGrammar
            : boost:proto::subscript<
                 boost::proto::arg<
                    boost::proto::unary_expr<
                       MIRV::Builder::Else,
                       ConstructBinaryTemporary<IfGrammar>
                    >,
                 >,
                 StatementGrammar
              > {};
#endif
#if 0
      // Here is the grammar for while_ statements
      // matches while_(e1)[e2]
      struct WhileGrammar
            : boost::proto::subscript<
                 boost::proto::arg<
                    boost::proto::unary_expr<
                       MIRV::Builder::While,
                       ExpressionGrammar
                    >,
                 >,
                 StatementGrammar
              > {};

      // Here is the grammar for do_ statements

      // A do_ "operator"
      boost::proto::terminal<Do>::type const do_ = {{}};

      // matches do[e1]
      struct DoGrammar
            : boost::proto::subscript<
                 MIRV::Builder::Do,
                 StatementGrammar
              > {};

      // A while_ "operator"
      template<typename Expr>
      struct Expression<Expr
         , typename enable_if<matches<Expr, DoGrammar> >::type
      >
      : extends<Expr, Expression<Expr>, Domain>
      {
         explicit Expression(Expr const &expr = Expr())
               : extends<Expr, Expression<Expr>, Domain>(expr)
         {}

         using extends<Expr, Expression<Expr>, Domain>::operator =;

         template< typename A0 >
         typename proto::result_of::make_expr<
            WhileTag, Domain, Expression const, A0 const
            >::type const
         while_(A0 const &a0) const
         {
            return make_expr<WhileTag, Domain>(*this, a0);
         }
      };

      // Matches do_[e1].while(e2)
      struct DoWhileGrammar
            : binary_expr<
                 WhileTag,
                 proto::arg<DoGrammar>,
                 StatementGrammar
              > {};

#endif
      // TODO: Switch
   }
}

#endif
