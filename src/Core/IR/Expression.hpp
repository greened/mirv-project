#ifndef mirv_Core_IR_Expression_hpp
#define mirv_Core_IR_Expression_hpp

#include <mirv/Core/IR/Inherit.hpp>
#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/IR/Type.hpp>
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

  template<typename Op> class Expression;

  namespace detail {
    template<typename Tag>
    struct VisitorBase<Expression<Tag> > {
      typedef typename Tag::VisitorBaseType VisitorBaseType;
    };
    template<typename Tag>
    struct BaseTypeOf<Expression<Tag> > {
      typedef typename Tag::BaseType BaseType;
    };
    template<>
    struct VisitorBase<Expression<Base> > {
      typedef Node<Base> VisitorBaseType;
    };
    template<>
    struct BaseTypeOf<Expression<Base> > {
      typedef Node<Base> BaseType;
    };
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
  public:
    /// The immediate base type of this expression, distinct from
    /// the base type that will be visited by an ExpressionVisitor.
    typedef ConstVisitable<
    Expression<Op>,
    ConstExpressionVisitor,
    ExpressionVisitor
    > BaseType;
    typedef typename Op::VisitorBaseType VisitorBaseType;

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
      typename ptr<Expression<Op> >::type result(new Expression<Op>(a1));
      result->setParents();
      return result;
    }

    template<typename A1, typename A2>
    static typename ptr<Expression<Op> >::type
    make(A1 a1, A2 a2) {
      typename ptr<Expression<Op> >::type result(new Expression<Op>(a1, a2));
      result->setParents();
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
      : public ConstVisitable<
    Expression<Base>,
    ConstExpressionVisitor,
    ExpressionVisitor
    > {
  private:
    virtual Expression<Base> *cloneImpl(void) = 0;

  public:
    typedef ptr<Symbol<Type<TypeBase>>>::const_type TypePtr;
    Expression<Base>(void) {}

    ptr<Expression<Base> >::type clone(void) {
      ptr<Expression<Base> >::type expr(cloneImpl());
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
      typedef ptr<Child>::type ChildPtr;
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
  class Expression<Inner<detail::InnerExpressionTraits> > : public Inner<detail::InnerExpressionTraits>::BaseType {
  public:
    typedef Expression<Base> VisitorBaseType;
    TypePtr type(void) const {
      return (*begin())->type();
    }
  };

  class InnerExpressionBase : public Expression<Inner<detail::InnerExpressionTraits> > {
  public:
    typedef Expression<Inner<detail::InnerExpressionTraits> > BaseType;
    typedef BaseType::VisitorBaseType VisitorBaseType;
  };
  
  /// This is the implementation of inner expressions.  It is
  /// inherited from once in the hierarchy for any inner expressions.
  /// This holds the child pointers and other data necessary for inner
  /// expressions.
  class InnerExpression : public InnerImpl<
    Expression<Base>,
    Virtual<InnerExpressionBase>
    > {
  public:
    typedef InnerImpl<Expression<Base>, Virtual<InnerExpressionBase> > BaseType;
    typedef Expression<Base> VisitorBaseType;

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
    InnerExpression(ChildPtr Child1,
		    ChildPtr Child2) : BaseType(Child1, Child2) {}
  };

  /// This is an expression with no children.
  class LeafExpression : public LeafImpl<Expression<Base> > {
  public:
    typedef LeafImpl<Expression<Base> > BaseType;
    typedef Expression<Base> VisitorBaseType;
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
      enum {
        NumInitializers = 1
      };

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
    };

  public:
    typedef Interface BaseType;
    typedef InnerExpression VisitorBaseType;
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
      enum {
        NumInitializers = 2
      };

      Interface(ChildPtr Child1,
                ChildPtr Child2) : InterfaceBaseType(Child1, Child2) {
        checkInvariant(Child1->type() == Child2->type(),
                       "Expression type mismatch");
      }

      /// Set the left child expression.
      void setLeftOperand(ChildPtr c) {
        checkInvariant(c->type() == getLeftOperand()->type(),
                       "Expression type mismatch");
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
    };

  public:
    typedef Interface BaseType;
    typedef InnerExpression VisitorBaseType;
  };

  // Expression property semantics

  /// Categorize all arithmetic expressions.
  class Arithmetic {
  public:
    typedef boost::mpl::int_<0> Order;

    /// Inherit virtually from the inner expression abstract interface.
    typedef Virtual<InnerExpressionBase> BaseType;
    typedef InnerExpressionBase VisitorBaseType;
  };

  /// Categorize all logical expressions.
  class Logical {
  public:
    typedef boost::mpl::int_<1> Order;
    /// Inherit virtually from the inner expression abstract interface.
    typedef Virtual<InnerExpressionBase> BaseType;
    typedef InnerExpressionBase VisitorBaseType;
  };

  /// Categorize all bitwise expressions.
  class Bitwise {
  public:
    typedef boost::mpl::int_<2> Order;
    /// Inherit virtually from the inner expression abstract interface.
    typedef Virtual<InnerExpressionBase> BaseType;
    typedef InnerExpressionBase VisitorBaseType;
  };

  /// Categorize all relational expressions.
  class Relational {
  public:
    typedef boost::mpl::int_<0> Order;
    /// Inherit virtually from the inner expression abstract interface.
    typedef Virtual<InnerExpressionBase> BaseType;
    typedef InnerExpressionBase VisitorBaseType;
  };

  /// Categorize all reference expressions.
  class Ref {
  public:
    typedef boost::mpl::int_<11> Order;
    /// Inherit virtually from the inner expression abstract interface.
    typedef Virtual<InnerExpressionBase> BaseType;
    typedef InnerExpressionBase VisitorBaseType;
  };

  /// Operands can be commuted.
  class Commutative {
  public:
    typedef boost::mpl::int_<3> Order;
    /// Inherit virtually from the inner expression abstract interface.
    typedef Virtual<InnerExpressionBase> BaseType;
    typedef InnerExpressionBase VisitorBaseType;
  };

  /// Operands can be reassociated.
  class Associative {
  public:
    typedef boost::mpl::int_<5> Order;
    /// Inherit virtually from the inner expression abstract interface.
    typedef Virtual<InnerExpressionBase> BaseType;
    typedef InnerExpressionBase VisitorBaseType;
  };

  /// This expression is transitive (a op b && b op c => a op c)
  class Transitive {
  public:
    typedef boost::mpl::int_<7> Order;
    /// Inherit virtually from the inner expression abstract interface.
    typedef Virtual<InnerExpressionBase> BaseType;
    typedef InnerExpressionBase VisitorBaseType;
  };

  /// This expression is reflexive.
  class Reflexive {
  public:
    typedef boost::mpl::int_<9> Order;
    /// Inherit virtually from the inner expression abstract interface.
    typedef Virtual<InnerExpressionBase> BaseType;
    typedef InnerExpressionBase VisitorBaseType;
  }; 

  namespace detail {
    template<int Num> struct constructor;

    template<>
    struct constructor<1> {
      template<typename ExprType>
      static typename ptr<ExprType>::type
      construct(boost::shared_ptr<ExprType> prototype) {
        typename ptr<ExprType>::type
          expr(ExprType::make(prototype->getOperand()->clone()));
        return expr;
      }
    };

    template<>
    struct constructor<2> {
      template<typename ExprType>
      static typename ptr<ExprType>::type
      construct(boost::shared_ptr<ExprType> prototype) {
        typename ptr<ExprType>::type
          expr(ExprType::make( prototype->getLeftOperand()->clone(),
                              prototype->getRightOperand()->clone()));
        return expr;
      }
    };
  }

  /// This is a metafunction to generate a scattered base class
  /// hierarchy of property expressions.  The Sequence is a sorted
  /// list of property tags and Root is the base type of the whole
  /// hierarchy.
  /// TODO: Get rid of this entirely and move it into Expression.
  template<typename Root, typename Tag, typename ...Property>
  class ExpressionBaseGenerator {
    typedef Root BaseType;

  public:
    class ExpressionInterface : public Root,
                                public Expression<Property>...,
                                public boost::enable_shared_from_this<Expression<Tag> > {
    private:
      Expression<Base> *cloneImpl(void) {
        typename ptr<Expression<Tag> >::type
          expr(detail::constructor<Expression<Tag>::NumInitializers>::
               construct(this->shared_from_this()));
        Expression<Tag> *result = expr.get();
        expr.reset();
        return result;
      }

    public:
      ExpressionInterface(void) {}

      template<typename A1>
      ExpressionInterface(A1 a1) : BaseType(a1) {}

      template<typename A1, typename A2>
      ExpressionInterface(A1 a1, A2 a2) : BaseType(a1, a2) {}

      template<typename A1, typename A2, typename A3>
      ExpressionInterface(A1 a1, A2 a2, A3 a3) : BaseType(a1, a2, a3) {}

      virtual void accept(ExpressionVisitor &) {
        error("ExpressionInterface::accept called");
      }
      virtual void accept(ConstExpressionVisitor &) const {
        error("ExpressionInterface::accept called");
      }

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base>>(this->shared_from_this());
      }
    };
    typedef ExpressionInterface type;
  };
}

#include <mirv/Core/IR/Expression.ipp>

#endif
