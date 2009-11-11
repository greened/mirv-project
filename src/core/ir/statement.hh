#ifndef mirv_core_ir_statement_hh
#define mirv_core_ir_statement_hh

#include <mirv/ir/node.hh>
#include <mirv/ir/expression.hh>

#include <mirv/filter/visitor.hh>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/int.hpp>

namespace mirv {
  struct StatementVisitor;

   // Statement property semantics

   /// Not all child statements may be executed
  class Conditional { typedef boost::mpl::int_<0> order; };

   /// Child statements may be executed multiple times
   class Iterative { typedef boost::mpl::int_<1> order; };

   /// Modifies program state
   class Mutating { typedef boost::mpl::int_<2> order; };

  namespace detail {
    // Provide an ordering for properties.
    template<typename T1, typename T2>
    struct StatementPropertyLess :
      public boost::mpl::less<typename T1::order, typename T2::order> {};
  }

   template<
      typename Tag,
      typename Base = typename BaseType<Tag>::type>
   class Statement: public Base {
   public:
     typedef typename Tag::visitor_base_type visitor_base_type;
     typedef typename boost::mpl::sort<
       typename Tag::properties,
       detail::StatementPropertyLess<boost::mpl::_1, boost::mpl::_2>
       >::type properties;
     virtual void accept(StatementVisitor &V);
   };

   typedef Statement<Base> BaseStatement;

  class InnerStatement : public InnerImpl<Virtual<BaseStatement> > {
  public:
    virtual void accept(StatementVisitor &V);
  };

  class LeafStatement : public LeafImpl<Virtual<BaseStatement> > {
  public:
    virtual void accept(StatementVisitor &V);

  };

  template<typename Property>
  class Statement<Property, void> : public virtual InnerStatement {};

   // A metafunction class to generate statement hierarchies.  This
   // makes sure that each property statement specifies Base as its
   // base type so that visit(Expreesion<>::base_type) works
   // correctly.
   class PropertyStatementGenerator {
   public:
      template<typename Property>
      struct apply {
         typedef Statement<Property, void> type;
      };
   };

   template<typename Sequence, typename Root>
   class StatementBaseGenerator {
   public:
      typedef typename Properties<PropertyStatementGenerator, Root, Sequence>::type
      type;
   };

   /// Statement semantics are somehow affected by expressions
   template<typename Stmt>
   class Controlled {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      class interface;
      typedef Stmt interface_base_type;
     typedef Stmt visitor_base_type;
     typedef sequence properties;

      class interface
            : public interface_base_type {
      protected:
         typedef ptr<BaseExpression>::type expression_ptr;
         typedef ptr<BaseExpression>::const_type const_expression_ptr;
         typedef std::list<expression_ptr> expression_list;

      private:
         expression_list expressions;

      protected:
         typedef expression_list::iterator expression_iterator;
         typedef expression_list::const_iterator const_expression_iterator;

         expression_iterator expression_begin(void) {
            return(expressions.begin());
         };
         const_expression_iterator expression_begin(void) const {
            return(expressions.begin());
         };

         expression_iterator expression_end(void) {
            return(expressions.end());
         };
         const_expression_iterator expression_end(void) const {
            return(expressions.end());
         };

         void expression_push_back(expression_ptr c) {
            expressions.push_back(c);
         };

         expression_ptr expression_front(void) {
            check_invariant(!expression_empty(),
                            "Attempt to get operand from empty node");
            return(expressions.front());
         };

         const_expression_ptr expression_front(void) const {
            check_invariant(!expression_empty(),
                            "Attempt to get operand from empty node");
            return(expressions.front());
         };

         expression_ptr expression_back(void) {
            check_invariant(!expression_empty(),
                            "Attempt to get operand from empty node");
            return(expressions.back());
         };

         const_expression_ptr expression_back(void) const {
            check_invariant(!expression_empty(),
                            "Attempt to get operand from empty node");
            return(expressions.back());
         };

 	 typedef expression_list::size_type size_type;
         size_type expression_size(void) const { return(expressions.size()); };

         bool expression_empty(void) const { return(expressions.empty()); };
      };

      typedef typename StatementBaseGenerator<sequence, interface>::type base_type;
   };

   template<typename Stmt>
   class SingleExpression {
   private:
      typedef boost::mpl::vector<> sequence;

   public:
      typedef Statement<Controlled<Stmt> > interface_base_type;
     typedef Statement<Controlled<Stmt> > visitor_base_type;
     typedef sequence properties;

      class interface
            : public interface_base_type {
      public:
         typedef typename interface_base_type::expression_ptr expression_ptr;
         typedef typename interface_base_type::const_expression_ptr
         const_expression_ptr;

         void set_expression(expression_ptr e) {
            if (this->expression_empty()) {
               expression_push_back(e);
            }
            else {
               *--this->expression_end() = e;
            }
         };

         expression_ptr get_expression(void) {
            check_invariant(!this->expression_empty()
			    && *this->expression_begin(),
                            "Attempt to get non-existent expression");
            return(*this->expression_begin());
         };

         const_expression_ptr get_expression(void) const {
            check_invariant(!this->expression_empty()
			    && *this->expression_begin(),
                            "Attempt to get non-existent expression");
            return(*this->expression_begin());
         };
      }; 

      typedef typename StatementBaseGenerator<sequence, interface>::type base_type;
   };
}

#endif
