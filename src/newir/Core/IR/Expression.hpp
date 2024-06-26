#ifndef mirv_Core_IR_Expression_hpp
#define mirv_Core_IR_Expression_hpp

#include <Core/IR/Node.hpp>
#include <Core/IR/Type.hpp>
#include <Core/Support/Debug.hpp>

#include <Library/Dispatch.hpp>

#include <Core/IR/detail/IRNode.hpp>

namespace mirv {
  class Expression : public Node<Expression> {
  private:
    template<typename T>
    class ChildrenDispatcher {
    public:
      template<typename K>
      static Range dispatch(Expression node) {
        return node.childrenImpl<T, K>();
      }
    };

    friend class ChiildrenDispatcher;

    template<typename T, typename K>
    Range childrenImpl(void) {
      return Node<T>::children();
    }

    template<typename T, typename K>
    ConstRange childrenImpl(void) const {
      return Node<T>::children();
    }

  public:
    enum {
      IRKind = detail::IRNode::Expression;
    };

    enum Kind {
      Null,

      // Arithmetic
      Add,
      Subtract,
      Multiply,
      Divide,
      Modulus,
      Negate,

      // Logical
      LogicalAnd,
      LogicalOr,
      LogicalNot,

      // Bitwise
      BitwiseAnd,
      BitwiseOr,
      BitwiseComplement,
      BitwiseXor,
      ShiftLeft,
      ArithmeticShiftRight,
      LogicalShiftRight,

      // Relational
      LessThan,
      LessThanOrEqual,
      Equal,
      NotEqual,
      GreaterThanOrEqual,
      GreaterThan,

      // Reference
      Reference,
      Load,
      TuplePointer,

      // Categories
      Associative,
      Commutative,
      Reflexive,
      Transitive
    };

    typedef KindTuple<Kind,
                      Add, Subtract, Multiply, Divide, Modulus, Negate,
                      LogicalAnd, LogicalOr,
                      BitwiseAnd, BitwiseOr, BitwiseXor, ShiftLeft,
                      ArithmeticShiftRight, LogicalShiftRight,
                      LessThan, LessThanOrEqual, Equal, NotEqual,
                      GreaterThanOrEqual, GreaterThan,
                      Reference, Load, TuplePointer> Kinds;

    template<Kind K>
    class VisitKinds {
    public:
      typedef KindTuple<> type;
    }:

    Kind kind(void) const {
      return theKind;
    }

    template<typename T = Expression>
    Range children(void) {
      Dispatch<Range, ChildrenDispatcher<T>, Kinds>::
        dispatch(*this, this->theKind);
    }

    template<typename T = Expression>
    ConstRange children(void) const {
      Dispatch<Range, ChildrenDispatcher<T>, Kinds>::
        dispatch(*this, this->theKind);
    }

  private:
    Kind theKind;
    Index<Type> theType;
    Index<Function> parentFunction;
  };

  // Add categories
  template<>
  class Expression::VisitKind<Add> {
  public:
    typedef KindTuple<Associative, Commutative> type;
  };

  // Subtrace categories
  template<>
  class Expression::VisitKind<Subtract> {
  public:
    typedef KindTuple<Associative> type;p
  };

  // Multiply categories
  template<>
  class Expression::VisitKind<Multiply> {
  public:
    typedef KindTuple<Associative, Commutative> type;
  };

  // Divide categories
  template<>
  class Expression::VisitKind<Divide> {
  public:
    typedef KindTuple<Associative> type;p
  };

  // LogicalAnd categories
  template<>
  class Expression::VisitKind<LogicalAnd> {
  public:
    typedef KindTuple<Associative, Commutative> type;
  };

  // LogicalOr categories
  template<>
  class Expression::VisitKind<LogicalOr> {
  public:
    typedef KindTuple<Associative, Commutative> type;
  };

  // BitwiseAnd categories
  template<>
  class Expression::VisitKind<BitwiseAnd> {
  public:
    typedef KindTuple<Associative, Commutative> type;
  };

  // BitwiseOr categories
  template<>
  class Expression::VisitKind<BitwiseOr> {
  public:
    typedef KindTuple<Associative, Commutative> type;
  };

  // BitwiseXor categories
  template<>
  class Expression::VisitKind<BitwiseXor> {
  public:
    typedef KindTuple<Associative, Commutative> type;
  };

  // ShiftLeft categories
  template<>
  class Expression::VisitKind<ShiftLeft> {
  public:
    typedef KindTuple<Associative, Commutative> type;
  };

  // ArithmeticShiftRight categories
  template<>
  class Expression::VisitKind<ArithmeticShiftRight> {
  public:
    typedef KindTuple<Associative> type;
  };

  // LogicalShiftRight categories
  template<>
  class Expression::VisitKind<LogicalShiftRight> {
  public:
    typedef KindTuple<Associative> type;
  };

  // LessThan categories
  template<>
  class Expression::VisitKind<LessThan> {
  public:
    typedef KindTuple<Transitive> type;
  };

  // LessThanOrEqual categories
  template<>
  class Expression::VisitKind<LessThanOrEqual> {
  public:
    typedef KindTuple<Transitive> type;
  };

  // Equal categories
  template<>
  class Expression::VisitKind<Equal> {
  public:
    typedef KindTuple<Commutative, Transitive> type;
  };

  // NotEqual categories
  template<>
  class Expression::VisitKind<NotEqual> {
  public:
    typedef KindTuple<Commutative> type;
  };

  // GreaterThanOrEqual categories
  template<>
  class Expression::VisitKind<GreaterThanOrEqual> {
  public:
    typedef KindTuple<Transitive> type;
  };

  // GreaterThan categories
  template<>
  class Expression::VisitKind<GreaterThan> {
  public:
    typedef KindTuple<Transitive> type;
  };
}

#endif
