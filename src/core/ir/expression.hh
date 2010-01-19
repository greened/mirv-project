#ifndef mirv_core_ir_expression_hh
#define mirv_core_ir_expression_hh

#include <mirv/core/ir/node.hh>
#include <mirv/core/ir/property.hh>

#include <mirv/core/mem/heap.hh>

#include <mirv/core/util/debug.hh>

#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
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
     struct ExpressionPropertyLess :
       public boost::mpl::less<typename T1::order, typename T2::order> {};
   }

  /// This is a metafunction class (generator) to transform a property
  /// tag into a property expression.  It is more convenient to reason
  /// about property tags and this is the glue that makes that
  /// possible.
  template<typename Op> class Expression;
  struct PropertyExpressionGenerator {
    /// A metafunction to produce a property expression given a
    /// property tag.
    template<typename Property>
    struct apply {
      typedef Expression<Property> type;
    };
  };

   struct ExpressionVisitor;

  /// This is the expression implementation for all expression types.
  /// Each expression type is an instance of this template
  /// (Expression<Add>, Expression<Or>, etc.).  It keeps all of the
  /// property and visitor logic in one place, hiding the gory details
  /// from the expression type tags and specific expression type
  /// interfaces.
    template<typename Op>
    class Expression : public Op::base_type {
    public:
      /// The immediate base type of this expression, distinct from
      /// the base type that will be visited by an ExpressionVisitor.
      typedef typename Op::base_type base_type;

      /// A list of sorted property tags.
      typedef typename boost::mpl::sort<
	typename Op::properties,
	detail::ExpressionPropertyLess<boost::mpl::_1, boost::mpl::_2>
	>::type properties;

    private:
      /// A list of property expressions generated from the list of
      /// property tags.  This defines the visitation order for
      /// property expressions.
      typedef typename boost::mpl::transform<properties, PropertyExpressionGenerator>::type property_expressions;

    public:
      // If there are properties, visit those first, otherwise visit
      // the specified visitor base type.

      /// The base type visited if the visit action for this
      /// expression is not implemented.  This is distinct from the
      /// expression's base type because there may be various
      /// intermediate glue base types (scattered inheritance
      /// generators, etc.) used to implement the expression class
      /// hierarchy.
      typedef typename boost::mpl::eval_if<
	boost::mpl::empty<properties>,
	boost::mpl::identity<typename Op::visitor_base_type>,
	boost::mpl::deref<typename boost::mpl::begin<property_expressions>::type>
	>::type visitor_base_type;

      /// The root type for all expressions.
      typedef Expression<Base> RootType;

    protected:
      Expression(void) {}
      template<typename A1>
      Expression(A1 a1) : base_type(a1) {}
      template<typename A1, typename A2>
      Expression(A1 a1, A2 a2) : base_type(a1, a2) {}

    public:
     template<typename A1>
     static typename ptr<Expression<Op> >::type
     make(A1 a1) {
       return typename ptr<Expression<Op> >::type(new Expression<Op>(a1));
     }

     template<typename A1, typename A2>
     static typename ptr<Expression<Op> >::type
     make(A1 a1, A2 a2) {
       return typename ptr<Expression<Op> >::type(new Expression<Op>(a1, a2));
     }

     virtual void accept(ExpressionVisitor &V);
   };

  /// A specialization for base expressions.  No property information
  /// is available.
    template<>
    class Expression<Base> : public Node<Base> {
    public:
     virtual void accept(ExpressionVisitor &V);
   };

   namespace detail {
     /// A traits class to define various properties of inner
     /// expressions such as child type, iterator types and other
     /// things.
     class InnerExpressionTraits {
     public:
       typedef Expression<Base> Child;
       typedef Expression<Base> base_type;

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

  /// This is an inner expression abstract interface.  It exists
  /// because we need to be able to inherit virtually from inner
  /// expressions (to allow property expression visitors to manipulate
  /// operands) but we do not want to force subclasses to explicitly
  /// initialize the inner expression object.  Separating the
  /// interface from the implementation solves that problem.
    template<>
    class Expression<Inner<detail::InnerExpressionTraits> > : public Inner<detail::InnerExpressionTraits>::base_type {
    public:
      typedef Expression<Base> visitor_base_type;
     virtual void accept(ExpressionVisitor &V);
   };

  class InnerExpressionBase : public Expression<Inner<detail::InnerExpressionTraits> > {};
  
  /// This is the implementation of inner expressions.  It is
  /// inherited from once in the hierarchy for any inner expressions.
  /// This holds the child pointers and other data necessary for inner
  /// expressions.
  class InnerExpression : public InnerImpl<Expression<Base>, VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type> {
   private:
     typedef InnerImpl<
       Expression<Base>,
      VisitedInherit1<ExpressionVisitor>::apply<
	Virtual<InnerExpressionBase>
      >::type> BaseType;

  public:
    typedef Expression<Base> visitor_base_type;

    InnerExpression(child_ptr Child) : BaseType(Child) {}
    InnerExpression(child_ptr Child1,
		    child_ptr Child2) : BaseType(Child1, Child2) {}
    virtual void accept(ExpressionVisitor &V);
  };

  /// This is an expression with no children.
  class LeafExpression : public LeafImpl<Expression<Base> > {
  public:
    typedef Expression<Base> visitor_base_type;

    virtual void accept(ExpressionVisitor &V);
  };

  /// The unary expression tag.  This implements the interface for
  /// unary expressions.
   class Unary {
   private:
      typedef InnerExpression interface_base_type;

   public:
      class interface;
      typedef interface base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;

     /// The base class for unary expressions, providing the interface
     /// and implementation for them.
      class interface : public interface_base_type {
      public:
	typedef Expression<Base> child_type;
         typedef ptr<child_type>::type child_ptr;
         typedef ptr<child_type>::const_type const_child_ptr;

	interface(child_ptr Child) : interface_base_type(Child) {}

	/// Set the child expression.
	void set_operand(child_ptr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         };

	/// Get the child expression.
         child_ptr get_operand(void) {
            return(front());
         };

	/// Get the child expression.
         const_child_ptr get_operand(void) const {
            return(front());
         };

	virtual void accept(ExpressionVisitor &V);
      };
   };

  /// The binary expression tag.  This provides the interface and
  /// implementation for binary expressions.  Convention: First list
  /// element is left operand, second is right operand
   class Binary {
   private:
      typedef InnerExpression interface_base_type;

   public:
     class interface;
     typedef interface base_type;
     typedef InnerExpression visitor_base_type;
     typedef boost::mpl::vector<> properties;

     /// The interface and implementation for binary expressions.
      class interface : public interface_base_type {
      public:
	interface(child_ptr Child1,
		  child_ptr Child2) : interface_base_type(Child1, Child2) {}

	/// Set the left child expression.
	void set_left_operand(child_ptr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         }

	/// Set the right child expression.
         void set_right_operand(child_ptr c) {
            if (empty()) {
               push_back(child_ptr());  // Placeholder for left operand
               push_back(c);
            }
            else {
               *--end() = c;
            }
         }

	/// Get the left child expression.
         child_ptr get_left_operand(void) {
            check_invariant(!empty(),
                            "Attempt to get operand from empty expression");
            return(front());
         }

	/// Get the left child expression.
         const_child_ptr get_left_operand(void) const {
            check_invariant(!empty(),
                            "Attempt to get operand from empty expression");
            return(front());
         }

	/// Get the right child expression.
         child_ptr get_right_operand(void) {
            check_invariant(size() > 1 && back(),
                            "Attempt to get missing operand from expression");
            return(back());
         }

	/// Get the right child expression.
         const_child_ptr get_right_operand(void) const {
            check_invariant(size() > 1 && back(),
                         "Attempt to get missing operand from expression");
            return(back());
         }
	virtual void accept(ExpressionVisitor &V);
      };
   };

   // Expression property semantics

  /// Categorize all arithmetic expressions.
   class Arithmetic {
   public:
     typedef boost::mpl::int_<0> order;

     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type base_type;
     typedef InnerExpressionBase visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };

  /// Categorize all logical expressions.
   class Logical {
   public:
     typedef boost::mpl::int_<1> order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type base_type;
     typedef InnerExpressionBase visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };

  /// Categorize all bitwise expressions.
   class Bitwise {
   public:
     typedef boost::mpl::int_<2> order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type base_type;
     typedef InnerExpressionBase visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };

  /// Categorize all relational expressions.
   class Relational {
   public:
     typedef boost::mpl::int_<0> order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type base_type;
     typedef InnerExpressionBase visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };

  /// Categorize all reference expressions.
   class Ref {
   public:
     typedef boost::mpl::int_<11> order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type base_type;
     typedef InnerExpressionBase visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };

  /// Operands can be commuted.
   class Commutative {
   public:
     typedef boost::mpl::int_<3> order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type base_type;
     typedef InnerExpressionBase visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };

  /// Operands can be reassociated.
   class Associative {
   public:
     typedef boost::mpl::int_<5> order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type base_type;
     typedef InnerExpressionBase visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };

  /// This expression is transitive (a op b && b op c => a op c)
  class Transitive {
   public:
     typedef boost::mpl::int_<7> order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type base_type;
     typedef InnerExpressionBase visitor_base_type;
     typedef boost::mpl::vector<> properties;
   };

  /// This expression is reflexive.
   class Reflexive {
   public:
     typedef boost::mpl::int_<9> order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type base_type;
     typedef InnerExpressionBase visitor_base_type;
     typedef boost::mpl::vector<> properties;
   }; 
 
  /// This is a metafunction to generate a scattered base class
  /// hierarchy of property expressions.  The Sequence is a sorted
  /// list of property tags and Root is the base type of the whole
  /// hierarchy.
  template<typename Sequence, typename Root>
   class ExpressionBaseGenerator {
   public:
    /// The metafunction result.
    typedef typename Properties<PropertyExpressionGenerator, Root, Sequence,
				VisitedInherit2<ExpressionVisitor> >::type
      type;

   };
}

#include <mirv/core/ir/expression.ii>

#endif
