#ifndef mirv_Core_IR_Statement_hpp
#define mirv_Core_IR_Statement_hpp

#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/IR/Inherit.hpp>
#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/IR/ExpressionFwd.hpp>
#include <mirv/Core/IR/Visitable.hpp>

#include <boost/mpl/empty_base.hpp>
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
  struct ConstStatementVisitor;

  template<typename Tag> class Statement;

  namespace detail {
    template<typename Tag>
    struct VisitorBase<Statement<Tag> > {
      typedef typename Tag::VisitorBaseType VisitorBaseType;
    };
    template<typename Tag>
    struct BaseTypeOf<Statement<Tag> > {
      typedef typename Tag::BaseType BaseType;
    };
    template<>
    struct VisitorBase<Statement<Base> > {
      typedef Node<Base> VisitorBaseType;
    };
    template<>
    struct BaseTypeOf<Statement<Base> > {
      typedef Node<Base> BaseType;
    };
  }

  /// This is the statement implementation for all statement types.
  /// Each statement type is an instance of this template
  /// (Statement<IfThen>, Statement<DoWhile>, etc.).  It keeps all of
  /// the property and visitor logic in one place, hiding the gory
  /// details from the statement type tags and specific statement type
  /// Interfaces.
  template<typename Tag>
  class Statement : public ConstVisitable<
    Statement<Tag>,
    ConstStatementVisitor,
    StatementVisitor
    > {
  public:
    /// The immediate base type of this statement, distinct from
    /// the base type that will be visited by a StatementVisitor.
    typedef ConstVisitable<
    Statement<Tag>,
    ConstStatementVisitor,
    StatementVisitor
    > BaseType;
    typedef typename Tag::VisitorBaseType VisitorBaseType;

  protected:
    Statement(void) {}
    template<typename A1>
    Statement(A1 a1) : BaseType(a1) {}
    template<typename A1, typename A2>
    Statement(A1 a1, A2 a2) : BaseType(a1, a2) {}
    template<typename A1, typename A2, typename A3>
    Statement(A1 a1, A2 a2, A3 a3) : BaseType(a1, a2, a3) {}

  public:
    static typename ptr<Statement<Tag> >::type
    make(void) {
      typename ptr<Statement<Tag> >::type p(new Statement<Tag>());
      return p;
    }

    template<typename A1>
    static typename ptr<Statement<Tag> >::type
    make(A1 a1) {
      typename ptr<Statement<Tag> >::type p(new Statement<Tag>(a1));
      p->setParents();
      return p;
    }

    template<typename A1, typename A2>
    static typename ptr<Statement<Tag> >::type
    make(A1 a1, A2 a2) {
      typename ptr<Statement<Tag> >::type p(new Statement<Tag>(a1, a2));
      p->setParents();
      return p;
    }

    template<typename A1, typename A2, typename A3>
    static typename ptr<Statement<Tag> >::type
    make(A1 a1, A2 a2, A3 a3) {
      typename ptr<Statement<Tag> >::type p(new Statement<Tag>(a1, a2, a3));
      p->setParents();
      return p;
    }
  };

  /// This anchors the Statement virtual table.
  template<>
  class Visitable<Statement<Base>, StatementVisitor> : public Node<Base> {
  public:
    virtual void accept(StatementVisitor &V);
  };

  /// This anchors the Statement virtual table.
  template<>
  class ConstVisitable<
    Statement<Base>,
    ConstStatementVisitor,
    StatementVisitor
    > : public Visitable<Statement<Base>, StatementVisitor> {
  public:
    using Visitable<Statement<Base>, StatementVisitor>::accept;

    virtual void accept(ConstStatementVisitor &V) const;
  };
  
  /// A specialization for base statements.  No property information
  /// is available.
  template<>
  class Statement<Base>
      : public virtual ConstVisitable<
    Statement<Base>,
    ConstStatementVisitor,
    StatementVisitor
    > {
  private:
    virtual Statement<Base> *cloneImpl(void) = 0;

  public:
    ptr<Statement<Base> >::type clone(void) {
      ptr<Statement<Base> >::type stmt(cloneImpl());
      return stmt;
    }
  };

  namespace detail {
    /// A traits class to define various properties of inner
    /// statements such as child type, iterator types and other
    /// things.
    class InnerStatementTraits {
    public:
      typedef Statement<Base> Child;
      
      typedef Virtual<Statement<Base> > BaseType;

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

  /// This is an inner statement abstract Interface.  It exists
  /// because we need to be able to inherit virtually from inner
  /// statements (to allow property statement visitors to manipulate
  /// operands) but we do not want to force subclasses to explicitly
  /// initialize the inner statement object.  Separating the
  /// Interface from the implementation solves that problem.
  class InnerStatementBase : public Statement<Inner<detail::InnerStatementTraits> > {
  };

  /// This is the implementation of inner statements.  It is
  /// inherited from once in the hierarchy for any inner statements.
  /// This holds the child pointers and other data necessary for inner
  /// statements.
  class InnerStatement : public InnerImpl<
    Statement<Base>,
    Virtual<InnerStatementBase>
    > {
  public:
    typedef InnerImpl<Statement<Base>, Virtual<InnerStatementBase> > BaseType;
    typedef Statement<Base> VisitorBaseType;

    InnerStatement(void) : BaseType() {}
    InnerStatement(ChildPtr Child) : BaseType(Child) {}
    InnerStatement(ChildPtr Child1,
                   ChildPtr Child2) : BaseType(Child1, Child2) {}
  };
 
  /// This is a statement with no children.
  class LeafStatement : public LeafImpl<Virtual<Statement<Base> > > {
  public:
    typedef LeafImpl<Virtual<Statement<Base> > > BaseType;
    typedef Statement<Base> VisitorBaseType;
  };

  // Statement property semantics

  /// Not all child statements may be executed
  class Conditional {
  public:
    typedef boost::mpl::int_<0> order;
    typedef Virtual<Statement<Base> > BaseType;
    typedef Statement<Base> VisitorBaseType;
  };

  /// Child statements may be executed multiple times
  class Iterative {
  public:
    typedef boost::mpl::int_<1> order;
    typedef Virtual<Statement<Base> > BaseType;
    typedef Statement<Base> VisitorBaseType;
  };

  /// Modifies program state
  class Mutating {
  public:
    typedef boost::mpl::int_<2> order;
    typedef Virtual<Statement<Base> > BaseType;
    typedef Statement<Base> VisitorBaseType;
  };

  /// This is a metafunction to generate a scattered base class
  /// hierarchy of property statements.  The Sequence is a sorted
  /// list of property tags and Root is the base type of the whole
  /// hierarchy.
  template<typename Root, typename Tag, typename ...Property>
  class StatementBaseGenerator {
    template<typename ...BaseProperty>
    class StatementBase 
        : public Root,
          public Statement<BaseProperty>... {
    public:
      StatementBase(void) : Root() {}
      template<typename ...Args>
        StatementBase(const Args &...args) : Root(args...) {}
      virtual void accept(StatementVisitor &) {
        error("StatementBase::accept called!");
      }
      virtual void accept(ConstStatementVisitor &) const {
        error("StatementBase::accept called!");
      }
    };

  public:
    typedef StatementBase<Property...> type;
  };

  /// Statement semantics are somehow affected by expressions
  class Controlled {
  private:
    class Interface : public virtual Statement<Base> {
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

      typedef ExpressionList::reverse_iterator ReverseExpressionIterator;
      typedef ExpressionList::const_reverse_iterator
      ConstReverseExpressionIterator;

      ExpressionIterator expressionBegin(void) {
        return(expressions.begin());
      };
      ConstExpressionIterator expressionBegin(void) const {
        return(expressions.begin());
      };

      ReverseExpressionIterator expressionRBegin(void) {
        return(expressions.rbegin());
      };
      ConstReverseExpressionIterator expressionRBegin(void) const {
        return(expressions.rbegin());
      };

      ExpressionIterator expressionEnd(void) {
        return(expressions.end());
      };
      ConstExpressionIterator expressionEnd(void) const {
        return(expressions.end());
      };

      ReverseExpressionIterator expressionREnd(void) {
        return(expressions.rend());
      };
      ConstReverseExpressionIterator expressionREnd(void) const {
        return(expressions.rend());
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
    typedef Statement<Base> VisitorBaseType;
    typedef Interface BaseType;
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
    typedef Statement<Controlled> VisitorBaseType;
    typedef Interface BaseType;
  };
}

#include <mirv/Core/IR/Statement.ipp>

#endif
