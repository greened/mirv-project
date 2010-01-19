#ifndef mirv_core_ir_statement_hh
#define mirv_core_ir_statement_hh

#include <mirv/core/builder/make.hh>
#include <mirv/core/ir/node.hh>
#include <mirv/core/ir/expression.hh>
#include <mirv/core/ir/inherit.hh>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/int.hpp>

namespace mirv {
   namespace detail {
     /// Provide an ordering for properties.  This ensures that class
     /// hierarchies are the same everywhere and thus that a
     /// particular property always has the same superclass tree.
     template<typename T1, typename T2>
     struct StatementPropertyLess :
       public boost::mpl::less<typename T1::order, typename T2::order> {};
   }

   struct StatementVisitor;

  template<typename Tag> class Statement;

  /// This is a metafunction class (generator) to transform a property
  /// tag into a property statement.  It is more convenient to reason
  /// about property tags and this is the glue that makes that
  /// possible.
  struct PropertyStatementGenerator {
    /// A metafunction to produce a property statement given a
    /// property tag.
    template<typename Property>
    struct apply {
      typedef Statement<Property> type;
    };
  };

  /// This is the statement implementation for all statement types.
  /// Each statement type is an instance of this template
  /// (Statement<IfThen>, Statement<DoWhile>, etc.).  It keeps all of
  /// the property and visitor logic in one place, hiding the gory
  /// details from the statement type tags and specific statement type
  /// interfaces.
   template<typename Tag>
   class Statement: public Tag::base_type {
   public:
      /// The immediate base type of this statement, distinct from
      /// the base type that will be visited by a StatementVisitor.
     typedef typename Tag::base_type BaseType;

      /// A list of sorted property tags.
     typedef typename boost::mpl::sort<
       typename Tag::properties,
       detail::StatementPropertyLess<boost::mpl::_1, boost::mpl::_2>
       >::type properties;
   private:
      /// A list of property statements generated from the list of
      /// property tags.  This defines the visitation order for
      /// property statements.
      typedef typename boost::mpl::transform<properties, PropertyStatementGenerator>::type property_statements;

   public:
      // If there are properties, visit those first, otherwise visit
      // the specified visitor base type.

      /// The base type visited if the visit action for this
      /// statement is not implemented.  This is distinct from the
      /// statement's base type because there may be various
      /// intermediate glue base types (scattered inheritance
      /// generators, etc.) used to implement the statement class
      /// hierarchy.
      typedef typename boost::mpl::eval_if<
	boost::mpl::empty<properties>,
	boost::mpl::identity<typename Tag::visitor_base_type>,
	boost::mpl::deref<typename boost::mpl::begin<property_statements>::type>
	>::type visitor_base_type;

   protected:
     Statement(void) {}
     template<typename A1>
     Statement(A1 a1) : BaseType(a1) {}
     template<typename A1, typename A2>
     Statement(A1 a1, A2 a2) : BaseType(a1, a2) {}
     template<typename A1, typename A2, typename A3>
     Statement(A1 a1, A2 a2, A3 a3) : BaseType(a1, a2, a3) {}

   public:
     template<typename A1>
     static typename ptr<Statement<Tag> >::type
     make(A1 a1) {
       typename ptr<Statement<Tag> >::type p(new Statement<Tag>(a1));
       return p;
     }

     template<typename A1, typename A2>
     static typename ptr<Statement<Tag> >::type
     make(A1 a1, A2 a2) {
       typename ptr<Statement<Tag> >::type p(new Statement<Tag>(a1, a2));
       return p;
     }

     template<typename A1, typename A2, typename A3>
     static typename ptr<Statement<Tag> >::type
     make(A1 a1, A2 a2, A3 a3) {
       typename ptr<Statement<Tag> >::type p(new Statement<Tag>(a1, a2, a3));
	return p;
     }

     virtual void accept(StatementVisitor &V);
   };

  /// A specialization for base statements.  No property information
  /// is available.
   template<>
   class Statement<Base> : public Node<Base> {
   public:
     virtual void accept(StatementVisitor &V);
   };

  namespace detail {
     /// A traits class to define various properties of inner
     /// statements such as child type, iterator types and other
     /// things.
    class InnerStatementTraits {
    public:
      typedef Statement<Base> Child;

      typedef VisitedInherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type base_type;

    private:
      typedef ptr<Child>::type child_ptr;
      typedef std::list<child_ptr> child_list;

    public:
      /// Make this compatible with certain standard algorithms.
      typedef const child_ptr & const_reference;

      typedef child_list::iterator iterator;
      typedef child_list::reverse_iterator reverse_iterator;
      typedef child_list::const_iterator const_iterator;
      typedef child_list::const_reverse_iterator const_reverse_iterator;

      typedef child_list::size_type size_type;
    };
  }

  /// This is an inner statement abstract interface.  It exists
  /// because we need to be able to inherit virtually from inner
  /// statements (to allow property statement visitors to manipulate
  /// operands) but we do not want to force subclasses to explicitly
  /// initialize the inner statement object.  Separating the
  /// interface from the implementation solves that problem.
  class InnerStatementBase : public Statement<Inner<detail::InnerStatementTraits> > {};

  /// This is the implementation of inner statements.  It is
  /// inherited from once in the hierarchy for any inner statements.
  /// This holds the child pointers and other data necessary for inner
  /// statements.
  class InnerStatement : public InnerImpl<Statement<Base>, VisitedInherit1<StatementVisitor>::apply<Virtual<InnerStatementBase> >::type> {
     typedef InnerImpl<
       Statement<Base>,
      VisitedInherit1<StatementVisitor>::apply<
	Virtual<InnerStatementBase>
      >::type> BaseType;
   public:
    InnerStatement(child_ptr Child) : BaseType(Child) {}
    InnerStatement(child_ptr Child1,
		   child_ptr Child2) : BaseType(Child1, Child2) {}
     virtual void accept(StatementVisitor &V);
   };
 
  /// This is a statement with no children.
  class LeafStatement : public LeafImpl<VisitedInherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type> {
   public:
     virtual void accept(StatementVisitor &V);

   };

   // Statement property semantics

    /// Not all child statements may be executed
   class Conditional {
    public:
     typedef boost::mpl::int_<0> order;
     typedef VisitedInherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type base_type;
     typedef Statement<Base> visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };

    /// Child statements may be executed multiple times
    class Iterative {
    public:
      typedef boost::mpl::int_<1> order;
      typedef VisitedInherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type base_type;
     typedef Statement<Base> visitor_base_type;
     typedef boost::mpl::vector<> properties;
    };

    /// Modifies program state
    class Mutating {
    public:
      typedef boost::mpl::int_<2> order;
      typedef VisitedInherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type base_type;
     typedef Statement<Base> visitor_base_type;
     typedef boost::mpl::vector<> properties;
    };

  /// This is a metafunction to generate a scattered base class
  /// hierarchy of property statements.  The Sequence is a sorted
  /// list of property tags and Root is the base type of the whole
  /// hierarchy.
    template<typename Sequence, typename Root>
    class StatementBaseGenerator {
    public:
      typedef typename Properties<PropertyStatementGenerator, Root, Sequence,
				  VisitedInherit2<StatementVisitor> >::type
       type;
    };

    /// Statement semantics are somehow affected by expressions
    class Controlled {
    private:
       typedef boost::mpl::vector<> sequence;

    public:
       class interface;
      typedef VisitedInherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type interface_base_type;
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
	 template<typename A1>
	 interface(A1 a1) {
	   expressions.push_back(a1);
	 }
	 template<typename A1, typename A2>
	 interface(A1 a1, A2 a2) {
	   expressions.push_back(a1);
	   expressions.push_back(a2);
	 }

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

  /// A statement with a single expression child.  It may have one of
  /// more children of other types.
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
	 template<typename A1>
	 interface(A1 a1) : interface_base_type(a1) {}

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
