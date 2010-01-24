#ifndef mirv_core_ir_Expression_hpp
#define mirv_core_ir_Expression_hpp

#include <mirv/core/ir/Node.hpp>
#include <mirv/core/ir/Property.hpp>

#include <mirv/core/mem/Heap.hpp>

#include <mirv/core/util/Debug.hpp>

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
       public boost::mpl::less<typename T1::Order, typename T2::Order> {};
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
    class Expression : public Op::BaseType {
    public:
      /// The immediate base type of this expression, distinct from
      /// the base type that will be visited by an ExpressionVisitor.
      typedef typename Op::BaseType BaseType;

      /// A list of sorted property tags.
      typedef typename boost::mpl::sort<
	typename Op::Properties,
	detail::ExpressionPropertyLess<boost::mpl::_1, boost::mpl::_2>
	>::type Properties;

    private:
      /// A list of property expressions generated from the list of
      /// property tags.  This defines the visitation order for
      /// property expressions.
      typedef typename boost::mpl::transform<Properties, PropertyExpressionGenerator>::type PropertyExpressions;

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
	boost::mpl::empty<Properties>,
	boost::mpl::identity<typename Op::VisitorBaseType>,
	boost::mpl::deref<typename boost::mpl::begin<PropertyExpressions>::type>
	>::type VisitorBaseType;

      /// The root type for all expressions.
      //typedef Expression<Base> RootType;

    protected:
      Expression(void) {}
      template<typename A1>
      Expression(A1 a1) : BaseType(a1) {}
      template<typename A1, typename A2>
      Expression(A1 a1, A2 a2) : BaseType(a1, a2) {}

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
       typedef Expression<Base> BaseType;

     private:
       typedef ptr<Child>::type ChildPtr;
       typedef std::list<ChildPtr> ChildList;

     public:
       /// Make this compatible with certain standard algorithms.
       typedef const ChildPtr & const_reference;

       typedef ChildList::iterator iterator;
       typedef ChildList::reverse_iterator reverse_iterator;
       typedef ChildList::const_iterator const_iterator;
       typedef ChildList::const_reverse_iterator const_reverse_iterator;

       typedef ChildList::size_type size_type;
     };
   }

  /// This is an inner expression abstract interface.  It exists
  /// because we need to be able to inherit virtually from inner
  /// expressions (to allow property expression visitors to manipulate
  /// operands) but we do not want to force subclasses to explicitly
  /// initialize the inner expression object.  Separating the
  /// interface from the implementation solves that problem.
    template<>
    class Expression<Inner<detail::InnerExpressionTraits> > : public Inner<detail::InnerExpressionTraits>::BaseType {
    public:
      typedef Expression<Base> VisitorBaseType;
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
    typedef Expression<Base> VisitorBaseType;

    InnerExpression(ChildPtr Child) : BaseType(Child) {}
    InnerExpression(ChildPtr Child1,
		    ChildPtr Child2) : BaseType(Child1, Child2) {}
    virtual void accept(ExpressionVisitor &V);
  };

  /// This is an expression with no children.
  class LeafExpression : public LeafImpl<Expression<Base> > {
  public:
    typedef Expression<Base> VisitorBaseType;

    virtual void accept(ExpressionVisitor &V);
  };

  /// The unary expression tag.  This implements the interface for
  /// unary expressions.
   class Unary {
   private:
      typedef InnerExpression InterfaceBaseType;

     /// The base class for unary expressions, providing the interface
     /// and implementation for them.
      class Interface : public InterfaceBaseType {
      public:
	typedef Expression<Base> ChildType;
         typedef ptr<ChildType>::type ChildPtr;
         typedef ptr<ChildType>::const_type ConstChildPtr;

	Interface(ChildPtr Child) : InterfaceBaseType(Child) {}

	/// Set the child expression.
	void setOperand(ChildPtr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         };

	/// Get the child expression.
         ChildPtr getOperand(void) {
            return(front());
         };

	/// Get the child expression.
         ConstChildPtr getOperand(void) const {
            return(front());
         };

	virtual void accept(ExpressionVisitor &V);
      };

   public:
     typedef Interface BaseType;
     typedef InnerExpression VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
   };

  /// The binary expression tag.  This provides the interface and
  /// implementation for binary expressions.  Convention: First list
  /// element is left operand, second is right operand
   class Binary {
   private:
      typedef InnerExpression InterfaceBaseType;

     /// The interface and implementation for binary expressions.
      class Interface : public InterfaceBaseType {
      public:
	Interface(ChildPtr Child1,
		  ChildPtr Child2) : InterfaceBaseType(Child1, Child2) {}

	/// Set the left child expression.
	void setLeftOperand(ChildPtr c) {
            if (empty()) {
               push_back(c);
            }
            else {
               *begin() = c;
            }
         }

	/// Set the right child expression.
         void setRightOperand(ChildPtr c) {
            if (empty()) {
               push_back(ChildPtr());  // Placeholder for left operand
               push_back(c);
            }
            else {
               *--end() = c;
            }
         }

	/// Get the left child expression.
         ChildPtr getLeftOperand(void) {
            checkInvariant(!empty(),
			   "Attempt to get operand from empty expression");
            return(front());
         }

	/// Get the left child expression.
         ConstChildPtr getLeftOperand(void) const {
            checkInvariant(!empty(),
			   "Attempt to get operand from empty expression");
            return(front());
         }

	/// Get the right child expression.
         ChildPtr getRightOperand(void) {
            checkInvariant(size() > 1 && back(),
			   "Attempt to get missing operand from expression");
            return(back());
         }

	/// Get the right child expression.
         ConstChildPtr getRightOperand(void) const {
	   checkInvariant(size() > 1 && back(),
			   "Attempt to get missing operand from expression");
            return(back());
         }
	virtual void accept(ExpressionVisitor &V);
      };

   public:
     typedef Interface BaseType;
     typedef InnerExpression VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
   };

   // Expression property semantics

  /// Categorize all arithmetic expressions.
   class Arithmetic {
   public:
     typedef boost::mpl::int_<0> Order;

     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type BaseType;
     typedef InnerExpressionBase VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
   };

  /// Categorize all logical expressions.
   class Logical {
   public:
     typedef boost::mpl::int_<1> Order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type BaseType;
     typedef InnerExpressionBase VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
   };

  /// Categorize all bitwise expressions.
   class Bitwise {
   public:
     typedef boost::mpl::int_<2> Order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type BaseType;
     typedef InnerExpressionBase VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
   };

  /// Categorize all relational expressions.
   class Relational {
   public:
     typedef boost::mpl::int_<0> Order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type BaseType;
     typedef InnerExpressionBase VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
   };

  /// Categorize all reference expressions.
   class Ref {
   public:
     typedef boost::mpl::int_<11> Order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type BaseType;
     typedef InnerExpressionBase VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
   };

  /// Operands can be commuted.
   class Commutative {
   public:
     typedef boost::mpl::int_<3> Order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type BaseType;
     typedef InnerExpressionBase VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
   };

  /// Operands can be reassociated.
   class Associative {
   public:
     typedef boost::mpl::int_<5> Order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type BaseType;
     typedef InnerExpressionBase VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
   };

  /// This expression is transitive (a op b && b op c => a op c)
  class Transitive {
   public:
     typedef boost::mpl::int_<7> Order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type BaseType;
     typedef InnerExpressionBase VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
   };

  /// This expression is reflexive.
   class Reflexive {
   public:
     typedef boost::mpl::int_<9> Order;
     /// Inherit virtually from the inner expression abstract interface.
     typedef VisitedInherit1<ExpressionVisitor>::apply<Virtual<InnerExpressionBase> >::type BaseType;
     typedef InnerExpressionBase VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
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

#include <mirv/core/ir/Expression.ipp>

#endif
