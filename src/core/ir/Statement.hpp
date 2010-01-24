#ifndef mirv_core_ir_Statement_hpp
#define mirv_core_ir_Statement_hpp

#include <mirv/core/builder/Make.hpp>
#include <mirv/core/ir/Node.hpp>
#include <mirv/core/ir/Expression.hpp>
#include <mirv/core/ir/Inherit.hpp>

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
  /// Interfaces.
   template<typename Tag>
   class Statement: public Tag::BaseType {
   public:
     /// The immediate base type of this statement, distinct from
     /// the base type that will be visited by a StatementVisitor.
     typedef typename Tag::BaseType BaseType;

      /// A list of sorted property tags.
     typedef typename boost::mpl::sort<
       typename Tag::Properties,
       detail::StatementPropertyLess<boost::mpl::_1, boost::mpl::_2>
       >::type Properties;
   private:
      /// A list of property statements generated from the list of
      /// property tags.  This defines the visitation order for
      /// property statements.
      typedef typename boost::mpl::transform<Properties, PropertyStatementGenerator>::type PropertyStatements;

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
	boost::mpl::empty<Properties>,
	boost::mpl::identity<typename Tag::VisitorBaseType>,
	boost::mpl::deref<typename boost::mpl::begin<PropertyStatements>::type>
	>::type VisitorBaseType;

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

      typedef VisitedInherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type BaseType;

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

  /// This is an inner statement abstract Interface.  It exists
  /// because we need to be able to inherit virtually from inner
  /// statements (to allow property statement visitors to manipulate
  /// operands) but we do not want to force subclasses to explicitly
  /// initialize the inner statement object.  Separating the
  /// Interface from the implementation solves that problem.
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
    InnerStatement(ChildPtr Child) : BaseType(Child) {}
    InnerStatement(ChildPtr Child1,
		   ChildPtr Child2) : BaseType(Child1, Child2) {}
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
     typedef VisitedInherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type BaseType;
     typedef Statement<Base> VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
   };

    /// Child statements may be executed multiple times
    class Iterative {
    public:
      typedef boost::mpl::int_<1> order;
      typedef VisitedInherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type BaseType;
     typedef Statement<Base> VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
    };

    /// Modifies program state
    class Mutating {
    public:
      typedef boost::mpl::int_<2> order;
      typedef VisitedInherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type BaseType;
     typedef Statement<Base> VisitorBaseType;
     typedef boost::mpl::vector<> Properties;
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
      typedef VisitedInherit1<StatementVisitor>::apply<Virtual<Statement<Base> > >::type InterfaceBaseType;

       class Interface : public virtual InterfaceBaseType {
       protected:
	 typedef ptr<Expression<Base> >::type ExpressionPtr;
	 typedef ptr<Expression<Base> >::const_type ConstExpressionPtr;
	  typedef std::list<ExpressionPtr> ExpressionList;

       private:
	  ExpressionList expressions;

       protected:
	 template<typename A1>
	 Interface(A1 a1) {
	   expressions.push_back(a1);
	 }
	 template<typename A1, typename A2>
	 Interface(A1 a1, A2 a2) {
	   expressions.push_back(a1);
	   expressions.push_back(a2);
	 }

	  typedef ExpressionList::iterator ExpressionIterator;
	  typedef ExpressionList::const_iterator ConstExpressionIterator;

	  ExpressionIterator expressionBegin(void) {
	     return(expressions.begin());
	  };
	  ConstExpressionIterator expressionBegin(void) const {
	     return(expressions.begin());
	  };

	  ExpressionIterator expressionEnd(void) {
	     return(expressions.end());
	  };
	  ConstExpressionIterator expressionEnd(void) const {
	     return(expressions.end());
	  };

	  void expressionPushBack(ExpressionPtr c) {
	     expressions.push_back(c);
	  };

	  ExpressionPtr expressionFront(void) {
	     checkInvariant(!expressionEmpty(),
			    "Attempt to get operand from empty node");
	     return(expressions.front());
	  };

	  ConstExpressionPtr expressionFront(void) const {
	     checkInvariant(!expressionEmpty(),
			    "Attempt to get operand from empty node");
	     return(expressions.front());
	  };

	  ExpressionPtr expressionBack(void) {
	     checkInvariant(!expressionEmpty(),
			    "Attempt to get operand from empty node");
	     return(expressions.back());
	  };

	  ConstExpressionPtr expressionBack(void) const {
	     checkInvariant(!expressionEmpty(),
			    "Attempt to get operand from empty node");
	     return(expressions.back());
	  };

	  typedef ExpressionList::size_type size_type;
	  size_type expressionSize(void) const { return(expressions.size()); };

	  bool expressionEmpty(void) const { return(expressions.empty()); };
       };

    public:
      typedef boost::mpl::vector<> Properties;
      typedef Statement<Base> VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
    };

  /// A statement with a single expression child.  It may have one of
  /// more children of other types.
    class SingleExpression { 
    private: 
      typedef Statement<Controlled> InterfaceBaseType;

       class Interface : public InterfaceBaseType {
       public:
	 template<typename A1>
	 Interface(A1 a1) : InterfaceBaseType(a1) {}

	 typedef InterfaceBaseType::ExpressionPtr ExpressionPtr;
	 typedef InterfaceBaseType::ConstExpressionPtr
         ConstExpressionPtr;

         void setExpression(ExpressionPtr e) {
	   if (this->expressionEmpty()) {
	     expressionPushBack(e);
	   }
	   else {
	     *--this->expressionEnd() = e;
	   }
         }

         ExpressionPtr getExpression(void) {
	   checkInvariant(!this->expressionEmpty()
			  && *this->expressionBegin(),
			  "Attempt to get non-existent expression");
	   return(*this->expressionBegin());
         }

         ConstExpressionPtr getExpression(void) const {
            checkInvariant(!this->expressionEmpty()
			   && *this->expressionBegin(),
			   "Attempt to get non-existent expression");
            return(*this->expressionBegin());
         }
      }; 

    public:
      typedef boost::mpl::vector<> Properties;
      typedef Statement<Controlled> VisitorBaseType;
      typedef StatementBaseGenerator<Properties, Interface>::type BaseType;
   };
}

#include <mirv/core/ir/Statement.ipp>

#endif
