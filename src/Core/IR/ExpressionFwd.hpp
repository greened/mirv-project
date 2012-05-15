#ifndef mirv_core_ir_ExpressionFwd_hpp
#define mirv_core_ir_ExpressionFwd_hpp

#include <mirv/Core/IR/NodeFwd.hpp>
#include <mirv/Core/IR/Inherit.hpp>
#include <mirv/Core/IR/Visitable.hpp>
#include <mirv/Core/Filter/ConstExpressionVisitorFwd.hpp>
#include <mirv/Core/Filter/ExpressionVisitorFwd.hpp>

namespace mirv {
  template<typename Op>
  class Expression;

  class InnerExpressionBase;
  class InnerExpression;
  class LeafExpression;
  class Unary;
  class Binary;

  class Arithmetic;
  class Logical;
  class Bitwise;
  class Relational;
  class Ref;

  class Commutative;
  class Associative;
  class Transitive;
  class Reflexive;

  namespace detail {
    /// Define the vitiation base type for an expression.
    template<typename Tag>
    struct VisitorBaseTypeOfExpression {};

    template<typename Tag>
    struct VisitorBaseTypeOf<Expression<Tag> > {
      typedef typename VisitorBaseTypeOfExpression<Tag>::VisitorBaseType VisitorBaseType;
    };

    /// Define the base type of a general expression.
    template<typename Tag>
    struct BaseTypeOfExpression {};

    template<typename Tag>
    struct BaseTypeOf<Expression<Tag> > {
      typedef typename BaseTypeOfExpression<Tag>::BaseType BaseType;
    };

    /// Define the visitation base type for base expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Base> {
      typedef Node<Base> VisitorBaseType;
    };
    /// Define the base type of base expressions.
    template<>
    struct BaseTypeOfExpression<Base> {
      typedef ConstVisitable<
        Expression<Base>,
        ConstExpressionVisitor,
        ExpressionVisitor
        > BaseType;
    };

    class InnerExpressionTraits;

    /// Define the visitation base type for inner expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Inner<InnerExpressionTraits> > {
      typedef Expression<Base> VisitorBaseType;
    };
    /// Define the base type of inner expressions.
    template<>
    struct BaseTypeOfExpression<Inner<InnerExpressionTraits> > {
      typedef InnerInterface<InnerExpressionTraits> BaseType;
    };

    /// Define the visitation base type for inner expressions.
    template<>
    struct VisitorBaseTypeOf<InnerExpressionBase> {
      typedef VisitorBaseTypeOfExpression<Inner<InnerExpressionTraits> >::VisitorBaseType VisitorBaseType;
    };
    /// Define the base type of inner expressions.
    template<>
    struct BaseTypeOf<InnerExpressionBase> {
      typedef Expression<Inner<InnerExpressionTraits> > BaseType;
    };

    /// Define the visitation base type for inner expressions.
    template<>
    struct VisitorBaseTypeOf<InnerExpression> {
      typedef Expression<Base> VisitorBaseType;
    };
    /// Define the base type of inner expressions.
    template<>
    struct BaseTypeOf<InnerExpression> {
      typedef InnerImpl<Expression<Base>, Virtual<InnerExpressionBase> > BaseType;
    };

    /// Define the visitation base type for leaf expressions.
    template<>
    struct VisitorBaseTypeOf<LeafExpression> {
      typedef Expression<Base> VisitorBaseType;
    };
    /// Define the base type of leaf expressions.
    template<>
    struct BaseTypeOf<LeafExpression> {
      typedef LeafImpl<Expression<Base> > BaseType;
    };

    class UnaryInterface;

    /// Define the visitation base type for unary expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Unary> {
      typedef InnerExpression VisitorBaseType;
    };
    /// Define the base type of unary expressions.
    template<>
    struct BaseTypeOfExpression<Unary> {
      typedef UnaryInterface BaseType;
    };

    class BinaryInterface;

    /// Define the visitation base type for binary expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Binary> {
      typedef InnerExpression VisitorBaseType;
    };
    /// Define the base type of binary expressions.
    template<>
    struct BaseTypeOfExpression<Binary> {
      typedef BinaryInterface BaseType;
    };

    /// Define the visitation base type for arithmetic expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Arithmetic> {
      typedef InnerExpressionBase VisitorBaseType;
    };
    /// Define the base type of aritthmetic expressions.
    template<>
    struct BaseTypeOfExpression<Arithmetic> {
      typedef Virtual<InnerExpressionBase> BaseType;
    };

    /// Define the visitation base type for logical expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Logical> {
      typedef InnerExpressionBase VisitorBaseType;
    };
    /// Define the base type of logical expressions.
    template<>
    struct BaseTypeOfExpression<Logical> {
      typedef Virtual<InnerExpressionBase> BaseType;
    };

    /// Define the visitation base type for bitwise expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Bitwise> {
      typedef InnerExpressionBase VisitorBaseType;
    };
    /// Define the base type of bitwise expressions.
    template<>
    struct BaseTypeOfExpression<Bitwise> {
      typedef Virtual<InnerExpressionBase> BaseType;
    };

    /// Define the visitation base type for relational expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Relational> {
      typedef InnerExpressionBase VisitorBaseType;
    };
    /// Define the base type of relational expressions.
    template<>
    struct BaseTypeOfExpression<Relational> {
      typedef Virtual<InnerExpressionBase> BaseType;
    };

    /// Define the visitation base type for ref expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Ref> {
      typedef InnerExpressionBase VisitorBaseType;
    };
    /// Define the base type of ref expressions.
    template<>
    struct BaseTypeOfExpression<Ref> {
      typedef Virtual<InnerExpressionBase> BaseType;
    };

    /// Define the visitation base type for commutative expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Commutative> {
      typedef InnerExpressionBase VisitorBaseType;
    };
    /// Define the base type of commutative expressions.
    template<>
    struct BaseTypeOfExpression<Commutative> {
      typedef Virtual<InnerExpressionBase> BaseType;
    };

    /// Define the visitation base type for associative expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Associative> {
      typedef InnerExpressionBase VisitorBaseType;
    };
    /// Define the base type of associative expressions.
    template<>
    struct BaseTypeOfExpression<Associative> {
      typedef Virtual<InnerExpressionBase> BaseType;
    };

    /// Define the visitation base type for transitive expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Transitive> {
      typedef InnerExpressionBase VisitorBaseType;
    };
    /// Define the base type of transitive expressions.
    template<>
    struct BaseTypeOfExpression<Transitive> {
      typedef Virtual<InnerExpressionBase> BaseType;
    };

    /// Define the visitation base type for reflexive expressions.
    template<>
    struct VisitorBaseTypeOfExpression<Reflexive> {
      typedef InnerExpressionBase VisitorBaseType;
    };
    /// Define the base type of reflexive expressions.
    template<>
    struct BaseTypeOfExpression<Reflexive> {
      typedef Virtual<InnerExpressionBase> BaseType;
    };
  }

  namespace detail {
    template<int Num> struct constructor;

    /// This is a functor to clone a unary expression.
    template<>
    struct constructor<1> {
      template<typename ExprType>
      static ptr<ExprType>
      construct(boost::shared_ptr<ExprType> prototype) {
        ptr<ExprType>
          expr(ExprType::make(prototype->getOperand()->clone()));
        return expr;
      }
    };

    /// This is a helper functor to clone a binary expression.
    template<>
    struct constructor<2> {
      template<typename ExprType>
      static ptr<ExprType>
      construct(boost::shared_ptr<ExprType> prototype) {
        ptr<ExprType>
          expr(ExprType::make(prototype->getLeftOperand()->clone(),
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
    /// This is the base class for all Expression interfaces.  It
    /// contains a lot of common code used to initialize and clone
    /// expressions, making higher-level expression implementations
    /// simpler.
    class ExpressionInterface
        : public Root,
          public Expression<Property>...,
          public boost::enable_shared_from_this<Expression<Tag> > {
    private:
      Expression<Base> *cloneImpl(void) {
        ptr<Expression<Tag> >
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

      ptr<Node<Base> > getSharedHandle(void) {
        return fast_cast<Node<Base>>(this->shared_from_this());
      }
      ptr<const Node<Base> > getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(this->shared_from_this());
      }
    };
    typedef ExpressionInterface type;
  };
}

#endif
