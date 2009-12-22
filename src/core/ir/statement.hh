#ifndef mirv_core_ir_statement_hh
#define mirv_core_ir_statement_hh

#include <mirv/core/ir/node.hh>
#include <mirv/core/ir/expression.hh>
#include <mirv/core/ir/inherit.hh>

#include <mirv/core/filter/visitor.hh>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/int.hpp>

namespace mirv {
   namespace detail {
     // Provide an ordering for properties.
     template<typename T1, typename T2>
     struct StatementPropertyLess :
       public boost::mpl::less<typename T1::order, typename T2::order> {};
   }

   struct StatementVisitor;

  template<typename Tag> class Statement;

    // A metafunction class to generate statement hierarchies.  This
    // makes sure that each property statement specifies Base as its
    // base type so that visit(Expreesion<>::base_type) works
    // correctly.
  struct PropertyStatementGenerator {
    template<typename Property>
    struct apply {
      typedef Statement<Property> type;
    };
  };

   template<typename Tag>
   class Statement: public Tag::base_type {
   public:
     typedef typename boost::mpl::sort<
       typename Tag::properties,
       detail::StatementPropertyLess<boost::mpl::_1, boost::mpl::_2>
       >::type properties;
   private:
      typedef typename boost::mpl::transform<properties, PropertyStatementGenerator>::type property_statements;

   public:
      // If there are properties, visit those first, otherwise visit
      // the specified visitor base type.
      typedef typename boost::mpl::eval_if<
	boost::mpl::empty<properties>,
	boost::mpl::identity<typename Tag::visitor_base_type>,
	boost::mpl::deref<typename boost::mpl::begin<property_statements>::type>
	>::type visitor_base_type;

     template<typename A1>
     static typename ptr<Statement<Tag> >::type
     make(A1 a1) {
       return new Statement<Tag>(typename Tag::base_type(a1));
     }

     template<typename A1, typename A2>
     static typename ptr<Statement<Tag> >::type
     make(A1 a1, A2 a2) {
       return new Statement<Tag>(typename Tag::base_type(a1, a2));
     }

     virtual void accept(StatementVisitor &V);
   };

   template<>
   class Statement<Base> : public Node<Base> {
   public:
     virtual void accept(StatementVisitor &V);
   };

  namespace detail {
    class InnerStatementTraits {
    public:
      typedef Statement<Base> Child;
      typedef Inherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type base_type;

    private:
      typedef ptr<Child>::type child_ptr;
      typedef std::list<child_ptr> child_list;

    public:
      typedef child_list::iterator iterator;
      typedef child_list::reverse_iterator reverse_iterator;
      typedef child_list::const_iterator const_iterator;
      typedef child_list::const_reverse_iterator const_reverse_iterator;

      typedef child_list::size_type size_type;
    };
  }

  class InnerStatement : public InnerImpl<Statement<Base>, Inherit1<StatementVisitor>::apply<Virtual<Statement<Inner<detail::InnerStatementTraits> > > >::type> {
   public:
     virtual void accept(StatementVisitor &V);
   };

  class LeafStatement : public LeafImpl<Inherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type> {
   public:
     virtual void accept(StatementVisitor &V);

   };

   // Statement property semantics

    /// Not all child statements may be executed
   class Conditional {
    public:
     typedef boost::mpl::int_<0> order;
     typedef Virtual<Statement<Base> > base_type;
     typedef Statement<Base> visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };

    /// Child statements may be executed multiple times
    class Iterative {
    public:
      typedef boost::mpl::int_<1> order;
      typedef Virtual<Statement<Base> > base_type;
     typedef Statement<Base> visitor_base_type;
     typedef boost::mpl::vector<> properties;
    };

    /// Modifies program state
    class Mutating {
    public:
      typedef boost::mpl::int_<2> order;
      typedef Virtual<Statement<Base> > base_type;
     typedef Statement<Base> visitor_base_type;
     typedef boost::mpl::vector<> properties;
    };

    template<typename Sequence, typename Root>
    class StatementBaseGenerator {
    public:
      typedef typename Properties<PropertyStatementGenerator, Root, Sequence,
				  Inherit2<StatementVisitor> >::type
       type;
    };

    /// Statement semantics are somehow affected by expressions
    class Controlled {
    private:
       typedef boost::mpl::vector<> sequence;

    public:
       class interface;
      typedef Inherit1<StatementVisitor>::apply<Virtual<Statement<Base> > > interface_base_type;
      typedef Statement<Base> visitor_base_type;
      typedef sequence properties;

       class interface
	     : public virtual interface_base_type {
       protected:
	 typedef ptr<Expression<Base> >::type expression_ptr;
	 typedef ptr<Expression<Base> >::const_type const_expression_ptr;
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

       typedef StatementBaseGenerator<sequence, interface>::type base_type;
    };

    class SingleExpression {
    private:
       typedef boost::mpl::vector<> sequence;

    public:
       typedef Statement<Controlled> interface_base_type;
      typedef Statement<Controlled> visitor_base_type;
      typedef sequence properties;

       class interface
	     : public interface_base_type {
       public:
	  typedef interface_base_type::expression_ptr expression_ptr;
	  typedef interface_base_type::const_expression_ptr
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

      typedef StatementBaseGenerator<sequence, interface>::type base_type;
   };
}

#include <mirv/core/ir/statement.ii>

#endif
