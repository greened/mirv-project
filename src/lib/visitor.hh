// A visitor implementation based on Loki's visitor.

#ifndef visitor_hh
#define visitor_hh

#include <boost/mpl/apply.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/placeholders.hpp>

namespace MIRV {
   template<typename Visited, typename Result = void>
   class visitor {
   public:
      typedef Result result_type;
      virtual result_type visit(Visited &) = 0;
   };

   // An acyclic visitor.  All visitor classes must derive from this.
   class acyclic_visitor_base {
   public:
      virtual ~acyclic_visitor_base(void) {};
   };

   // Exceptions
   class bad_visitor {};

   struct visitor_abort_function {
      template<typename Result, typename Visited>
      struct apply {
         struct type {
            Result operator()(Visited &v) const {
               throw bad_visitor();
            }
         };
      };
   };

   // All class heirarchies that are to be visited must derive from
   // this.  Metafunction must be an mpl-style metafunction class with
   // operator() implemented for a reference to acyclic_visitable_base
   // or the type of the target being visited.  The type returned by
   // mpl::apply<Metafunction, Result, Visited>> must be
   // default-constructable.
   template<
      typename Result = void,
      typename Metafunction = visitor_abort_function
      >
   class acyclic_visitable_base {
   public:
      typedef Result result_type;
      virtual ~acyclic_visitable_base(void) {};
      virtual result_type accept(acyclic_visitor_base &visitor) = 0;
   protected:
      template<typename Visited>
      static result_type accept_impl(Visited &target,
                                     acyclic_visitor_base &vtor) {
         if (visitor<Visited, result_type> *p =
             dynamic_cast<visitor<Visited, result_type> *>(&vtor)) {
            return(p->visit(target));
         }
         return(boost::mpl::apply2<Metafunction, result_type, Visited>::type()(target));
      }
   };

#define DEFINE_ACYCLIC_VISITABLE() \
   virtual result_type accept(acyclic_base_visitor &visitor) { \
      return(accept_impl(*this, visitor)); \
   }

   // A cyclic visitor
   template<typename Result, typename Typesequence>
   class cyclic_visitor
         : public boost::mpl::inherit_linearly<Typesequence,
      boost::mpl::inherit<boost::mpl::_1, visitor<boost::mpl::_2> > >::type {
   public:
      typedef Result result_type;
      template<class Visited>
      result_type visit(Visited &target) {
         visitor<Visited> &visitor = *this;
         return(visitor.visit(target));
      }
   };

#define DEFINE_CYCLIC_VISITABLE(VisitorType) \
   virtual result_type accept(VisitorType &visitor) { \
      visitor.visit(*this); \
   }
};

#endif
