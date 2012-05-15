#ifndef mirv_Core_IR_Expression_hpp
#define mirv_Core_IR_Expression_hpp

#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/Inherit.hpp>
#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>
#include <mirv/Core/IR/Visitable.hpp>

#include <mirv/Core/Memory/Heap.hpp>

#include <mirv/Core/Utility/Debug.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/mpl/empty_base.hpp>
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

  struct ExpressionVisitor;
  struct ConstExpressionVisitor;

  /// This is the expression implementation for all expression types.
  /// Each expression type is an instance of this template
  /// (Expression<Add>, Expression<Or>, etc.).  It keeps all of the
  /// property and visitor logic in one place, hiding the gory details
  /// from the expression type tags and specific expression type
  /// interfaces.
  template<typename Op>
  class Expression : public ConstVisitable<
    Expression<Op>,
    ConstExpressionVisitor,
    ExpressionVisitor
    > {
  private:
    /// The immediate base type of this expression, distinct from
    /// the base type that will be visited by an ExpressionVisitor.
    typedef ConstVisitable<
    Expression<Op>,
    ConstExpressionVisitor,
    ExpressionVisitor
    > BaseType;

  protected:
    Expression(void) {}
    template<typename A1>
    Expression(A1 a1) : BaseType(a1) {}
    template<typename A1, typename A2>
    Expression(A1 a1, A2 a2) : BaseType(a1, a2) {}
    template<typename A1, typename A2, typename A3>
    Expression(A1 a1, A2 a2, A3 a3) : BaseType(a1, a2, a3) {}
    template<typename A1, typename A2, typename A3, typename A4>
    Expression(A1 a1, A2 a2, A3 a3, A4 a4) : BaseType(a1, a2, a3, a4) {}

  public:
    template<typename A1>
    static ptr<Expression<Op> >
    make(A1 a1) {
      ptr<Expression<Op> > result(new Expression<Op>(a1));
      result->setParents();
      result->validate();
      return result;
    }

    template<typename A1, typename A2>
    static ptr<Expression<Op> >
    make(A1 a1, A2 a2) {
      ptr<Expression<Op> > result(new Expression<Op>(a1, a2));
      result->setParents();
      result->validate();
      return result;
    } 

    template<typename A1, typename A2, typename A3>
    static ptr<Expression<Op> >
    make(A1 a1, A2 a2, A3 a3) {
      ptr<Expression<Op> >
        result(new Expression<Op>(a1, a2, a3));
      result->setParents();
      result->validate();
      return result;
    } 

    template<typename A1, typename A2, typename A3, typename A4>
    static ptr<Expression<Op> >
    make(A1 a1, A2 a2, A3 a3, A4 a4) {
      ptr<Expression<Op> >
        result(new Expression<Op>(a1, a2, a3, a4));
      result->setParents();
      result->validate();
      return result;
    } 
  };

  /// This anchors the Expression virtual table.
  template<>
  class Visitable<Expression<Base>, ExpressionVisitor> : public Node<Base> {
  public:
    virtual void accept(ExpressionVisitor &V);
  };

  /// This anchors the Expression virtual table.
  template<>
  class ConstVisitable<
    Expression<Base>,
    ConstExpressionVisitor,
    ExpressionVisitor
    > : public Visitable<Expression<Base>, ExpressionVisitor> {
  public:
    using Visitable<Expression<Base>, ExpressionVisitor>::accept;
    virtual void accept(ConstExpressionVisitor &V) const;
  };

  /// A specialization for base expressions.  No property information
  /// is available.
  template<>
  class Expression<Base>
      : public detail::BaseTypeOfExpression<Base>::BaseType {
  private:
    virtual Expression<Base> *cloneImpl(void) = 0;

  public:
    typedef ptr<const Symbol<Type<TypeBase> > > TypePtr;
    Expression<Base>(void) {}

    ptr<Expression<Base> > clone(void) {
      ptr<Expression<Base> > expr(cloneImpl());
      return expr;
    }

    virtual TypePtr type(void) const = 0;
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
      typedef ptr<Child> ChildPtr;
      typedef std::list<ChildPtr> ChildList;

    public:
      /// Make this compatible with certain standard algorithms.
      typedef ChildPtr value_type;
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
  class Expression<Inner<detail::InnerExpressionTraits> >
      : public detail::BaseTypeOfExpression<Inner<detail::InnerExpressionTraits> >::BaseType {
  public:
    TypePtr type(void) const {
      return (*begin())->type();
    }
  };

  /// Define the base for expressions with children.
  class InnerExpressionBase : public detail::BaseTypeOf<InnerExpressionBase>::BaseType {};
  
  /// This is the implementation of inner expressions.  It is
  /// inherited from once in the hierarchy for any inner expressions.
  /// This holds the child pointers and other data necessary for inner
  /// expressions.
  class InnerExpression : public detail::BaseTypeOf<InnerExpression>::BaseType {
  private:
    typedef detail::BaseTypeOf<InnerExpression>::BaseType BaseType;

  protected:
    void setParents(void) {
      for (iterator i = begin();
           i != end();
           ++i) {
        (*i)->setParent(getSharedHandle());
      }
    }

  public:
    InnerExpression(ChildPtr Child) : BaseType(Child) {}
    InnerExpression(ConstChildPtr Child)
        : BaseType(boost::const_pointer_cast<ChildType>(Child)) {}

    InnerExpression(ChildPtr Child1,
		    ChildPtr Child2) : BaseType(Child1, Child2) {}

    InnerExpression(ConstChildPtr Child1,
		    ConstChildPtr Child2)
        : BaseType(boost::const_pointer_cast<ChildType>(Child1),
                   boost::const_pointer_cast<ChildType>(Child2)) {}

    InnerExpression(ChildPtr Child1,
		    ChildPtr Child2,
                    ChildPtr Child3) : BaseType(Child1, Child2, Child3) {}

    InnerExpression(ConstChildPtr Child1,
		    ConstChildPtr Child2,
                    ConstChildPtr Child3)
        : BaseType(boost::const_pointer_cast<ChildType>(Child1),
                   boost::const_pointer_cast<ChildType>(Child2),
                   boost::const_pointer_cast<ChildType>(Child3)) {}

    template<typename InputIterator>
    InnerExpression(ChildPtr Child1,
		    InputIterator start,
                    InputIterator end) : BaseType(Child1, start, end) {}
    template<typename InputIterator>
    InnerExpression(ConstChildPtr Child1,
		    InputIterator start,
                    InputIterator end)
        : BaseType(boost::const_pointer_cast<ChildType>(Child1), start, end) {}

    template<typename InputIterator>
    InnerExpression(ChildPtr Child1,
                    ChildPtr Child2,
		    InputIterator start,
                    InputIterator end) : BaseType(Child1, Child2, start, end) {}
    template<typename InputIterator>
    InnerExpression(ConstChildPtr Child1,
                    ConstChildPtr Child2,
		    InputIterator start,
                    InputIterator end)
        : BaseType(boost::const_pointer_cast<ChildType>(Child1),
                   boost::const_pointer_cast<ChildType>(Child2),
                   start,
                   end) {}
  };

  /// This is an expression with no children.
  class LeafExpression : public detail::BaseTypeOf<LeafExpression>::BaseType {};

  /// The unary expression tag.  This implements the interface for
  /// unary expressions.
  namespace detail {
    /// The base class for unary expressions, providing the interface
    /// and implementation for them.
    class UnaryInterface : public InnerExpression {
    public:
      enum {
        NumInitializers = 1
      };

      typedef Expression<Base> ChildType;
      typedef ptr<ChildType> ChildPtr;
      typedef ptr<const ChildType> ConstChildPtr;

      UnaryInterface(ChildPtr Child) : InnerExpression(Child) {}
      // Interface(ConstChildPtr Child) : InterfaceBaseType(Child) {}

      /// Set the child expression.
      void setOperand(ChildPtr c) {
        if (empty()) {
          push_back(c);
        }
        else {
          *begin() = c;
        }
        validate();
      };

      /// Get the child expression.
      ChildPtr getOperand(void) {
        return(front());
      };

      /// Get the child expression.
      ConstChildPtr getOperand(void) const {
        return(front());
      };
    };
  }
  
  class Unary {};

  namespace detail {
    /// The interface and implementation for binary expressions.
    class BinaryInterface : public InnerExpression {
    private:
      typedef InnerExpression  BaseType;
      void doValidation(void) const;

    public:
      enum {
        NumInitializers = 2
      };

      BinaryInterface(ChildPtr Child1,
                      ChildPtr Child2) : BaseType(Child1, Child2) {}

      /// Set the left child expression.
      void setLeftOperand(ChildPtr c) {
        if (empty()) {
          push_back(c);
        }
        else {
          *begin() = c;
        }
        validate();
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
        validate();
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
    };
  }

  /// The binary expression tag.  This provides the interface and
  /// implementation for binary expressions.  Convention: First list
  /// element is left operand, second is right operand
  class Binary {};

  // Expression property semantics

  /// Categorize all arithmetic expressions.
  class Arithmetic {
  public:
    typedef boost::mpl::int_<0> Order;
  };

  /// Categorize all logical expressions.
  class Logical {
  public:
    typedef boost::mpl::int_<1> Order;
  };

  /// Categorize all bitwise expressions.
  class Bitwise {
  public:
    typedef boost::mpl::int_<2> Order;
  };

  /// Categorize all relational expressions.
  class Relational {
  public:
    typedef boost::mpl::int_<0> Order;
  };

  /// Categorize all reference expressions.
  class Ref {
  public:
    typedef boost::mpl::int_<11> Order;
  };

  /// Operands can be commuted.
  class Commutative {
  public:
    typedef boost::mpl::int_<3> Order;
  };

  /// Operands can be reassociated.
  class Associative {
  public:
    typedef boost::mpl::int_<5> Order;
  };

  /// This expression is transitive (a op b && b op c => a op c)
  class Transitive {
  public:
    typedef boost::mpl::int_<7> Order;
  };

  /// This expression is reflexive.
  class Reflexive {
  public:
    typedef boost::mpl::int_<9> Order;
  };
}

#include <mirv/Core/IR/Expression.ipp>

#endif
