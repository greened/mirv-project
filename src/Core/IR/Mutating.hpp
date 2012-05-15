#ifndef mirv_Core_IR_Mutating_hpp
#define mirv_Core_IR_Mutating_hpp

#include <mirv/Core/IR/MutatingFwd.hpp>
#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/IR/VariableFwd.hpp>
#include <mirv/Core/IR/TypeFwd.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/bind/bind.hpp>

namespace mirv {
  namespace detail {
    /// Define the interface for statesments with two child
    /// expressions.
    class DualExpressionInterface : public Statement<Controlled> {
    private:
      typedef Statement<Controlled> BaseType;

    public:
      template<typename E1, typename E2>
      DualExpressionInterface(E1 e1, E2 e2) : BaseType(e1, e2) {}

      typedef BaseType::ExpressionPtr ExpressionPtr;
      typedef BaseType::ConstExpressionPtr ConstExpressionPtr;
         
      void setLeftExpression(ExpressionPtr e) {
        if (expressionEmpty()) {
          expressionPushBack(e);
        }
        else {
          *expressionBegin() = e;
        }
      }

      void setRightExpression(ExpressionPtr e) {
        if (expressionEmpty()) {
          expressionPushBack(ExpressionPtr()); // Placeholder
          expressionPushBack(e);
        }
        else {
          *--expressionEnd() = e;
        }
      }
         
      ExpressionPtr getLeftExpression(void) {
        checkInvariant(!expressionEmpty() && *expressionBegin(),
                       "Attempt to get non-existent expression");
        return(expressionFront());
      }

      ConstExpressionPtr getLeftExpression(void) const {
        checkInvariant(!expressionEmpty() && *expressionBegin(),
                       "Attempt to get non-existent expression");
        return(expressionFront());
      }

      ExpressionPtr getRightExpression(void) {
        checkInvariant(expressionSize() > 1 && expressionBack(),
                       "Attempt to get non-existent expression");
        return(expressionBack());
      }

      ConstExpressionPtr getRightExpression(void) const {
        checkInvariant(expressionSize() > 1 && expressionBack(),
                       "Attempt to get non-existent expression");
        return(expressionBack());
      }
    };
  }
  /// This is the interface to statements that have two child
  /// expressions.
  class DualExpression {};

  namespace detail {
    class PhiInterface : public Statement<Controlled>,
                         public LeafStatement,
                         public boost::enable_shared_from_this<Statement<Phi> > {
    private:
      ptr<Symbol<Variable> > theTarget;

      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      typedef ExpressionIterator iterator;
      typedef ConstExpressionIterator const_iterator;
      typedef ReverseExpressionIterator reverse_iterator;
      typedef ConstReverseExpressionIterator const_reverse_iterator;

      PhiInterface(ptr<Symbol<Variable> > target) :
          Statement<Controlled>(), LeafStatement(), theTarget(target) {}

      template<typename ...E>
      PhiInterface(ptr<Symbol<Variable> > target,
                E ...exprs) :
          Statement<Controlled>(exprs...), LeafStatement(), theTarget(target) {}

      typedef ptr<Symbol<Variable> > TargetPtr;
      typedef ptr<const Symbol<Variable> > ConstTargetPtr;

      typedef ExpressionPtr ChildPtr;
      typedef ConstExpressionPtr ConstChildPtr;

      ptr<Node<Base> > getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<const Node<Base> > getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }

      TargetPtr target(void) {
        return theTarget;
      }
      ConstTargetPtr target(void) const {
        return theTarget;
      }

      iterator begin(void) {
        return expressionBegin();
      }
      const_iterator begin(void) const {
        return expressionBegin();
      }
      reverse_iterator rbegin(void) {
        return expressionRBegin();
      }
      const_reverse_iterator rbegin(void) const {
        return expressionRBegin();
      }

      iterator end(void) {
        return expressionEnd();
      }
      const_iterator end(void) const {
        return expressionEnd();
      }
      reverse_iterator rend(void) {
        return expressionREnd();
      }
      const_reverse_iterator rend(void) const {
        return expressionREnd();
      }
    };
  }

  /// Specify the interface to phi statements.  Phis are statements
  /// only, not expressions.  This is to cleanly separate changes in
  /// program state from general computation.  Expression trees imply
  /// assignmnets to temporary variables at some level of translate,
  /// but we are not concerned about those.
  class Phi {};

  namespace detail {
    class StoreInterface : public Statement<DualExpression>,
                           public LeafStatement,
                           public boost::enable_shared_from_this<Statement<Store> > {
    private:
      Statement<Base> *cloneImpl(void);
      void doValidation(void) const;

    protected:
      void setParents(void);

    public:
      typedef ExpressionIterator iterator;
      typedef ConstExpressionIterator const_iterator;
      typedef ReverseExpressionIterator reverse_iterator;
      typedef ConstReverseExpressionIterator const_reverse_iterator;

      StoreInterface(ptr<Expression<Base> > e1,
                     ptr<Expression<Base> > e2);

      typedef ExpressionPtr ChildPtr;
      typedef ConstExpressionPtr ConstChildPtr;

      ptr<Node<Base> > getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<const Node<Base> > getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }

      iterator begin(void) {
        return expressionBegin();
      }
      const_iterator begin(void) const {
        return expressionBegin();
      }
      reverse_iterator rbegin(void) {
        return expressionRBegin();
      }
      const_reverse_iterator rbegin(void) const {
        return expressionRBegin();
      }

      iterator end(void) {
        return expressionEnd();
      }
      const_iterator end(void) const {
        return expressionEnd();
      }
      reverse_iterator rend(void) {
        return expressionREnd();
      }
      const_reverse_iterator rend(void) const {
        return expressionREnd();
      }
    };
  }

  /// Specify the interface to store statements.  Stores are
  /// statements only, not expressions.  This is to cleanly separate
  /// changes in program state from general computation.  Expression
  /// trees imply assignments to temporary variables at some level of
  /// translation, but we are not concerned about those.
  class Store {};

  namespace detail {
    /// Define the interface for Call statements.
    class CallInterface  : public Statement<Controlled>,
                           public LeafStatement,
                           public boost::enable_shared_from_this<Statement<Call> > {
    private:
      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      typedef ExpressionPtr ChildPtr;
      typedef ConstExpressionPtr ConstChildPtr;

      typedef ExpressionIterator iterator;
      typedef ConstExpressionIterator const_iterator;
      typedef ReverseExpressionIterator reverse_iterator;
      typedef ConstReverseExpressionIterator const_reverse_iterator;

      // Require a function reference to construct.
      CallInterface(ExpressionPtr function) : Statement<Controlled>(function) {}
      CallInterface(ExpressionPtr function, ExpressionPtr returnValue)
          : Statement<Controlled>(function, returnValue) {}

      template<typename InputIterator>
      CallInterface(ExpressionPtr function, InputIterator begin, InputIterator end)
          : Statement<Controlled>(function, begin, end) {}

      template<typename InputIterator>
      CallInterface(ExpressionPtr function,
                    ExpressionPtr returnValue,
                    InputIterator begin,
                    InputIterator end)
          : Statement<Controlled>(function, returnValue, begin, end) {}

      ptr<Node<Base> > getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<const Node<Base> > getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }

      // By convention, the first child is the expression referencing
      // the function.
      ExpressionPtr function(void) {
        checkInvariant(!expressionEmpty(), "No function for call");
        return expressionFront();
      }
      ConstExpressionPtr function(void) const {
        checkInvariant(!expressionEmpty(), "No function for call");
        return expressionFront();
      }

      ExpressionIterator argumentBegin(void) {
        checkInvariant(!expressionEmpty(), "No function for call");
        ExpressionIterator start = expressionBegin();
        std::advance(start, 1);
        return start;
      }
      ConstExpressionIterator argumentBegin(void)  const {
        checkInvariant(!expressionEmpty(), "No function for call");
        ConstExpressionIterator start = expressionBegin();
        std::advance(start, 1);
        return start;
      }
      ReverseExpressionIterator argumentRBegin(void) {
        checkInvariant(!expressionEmpty(), "No function for call");
        ReverseExpressionIterator start = expressionRBegin();
        std::advance(start, 1);
        return start;
      }
      ConstReverseExpressionIterator argumentRBegin(void) const {
        checkInvariant(!expressionEmpty(), "No function for call");
        ConstReverseExpressionIterator start = expressionRBegin();
        std::advance(start, 1);
        return start;
      }

      ExpressionIterator argumentEnd(void) {
        return expressionEnd();
      }
      ConstExpressionIterator argumentEnd(void) const {
        return expressionEnd();
      }
      ReverseExpressionIterator argumentREnd(void) {
        return expressionREnd();
      }
      ConstReverseExpressionIterator argumentREnd(void) const {
        return expressionREnd();
      }

      iterator begin(void) {
        return argumentBegin();
      }
      const_iterator begin(void) const {
        return argumentBegin();
      }
      reverse_iterator rbegin(void) {
        return argumentRBegin();
      }
      const_reverse_iterator rbegin(void) const {
        return argumentRBegin();
      }

      iterator end(void) {
        return argumentEnd();
      }
      const_iterator end(void) const {
        return argumentEnd();
      }
      reverse_iterator rend(void) {
        return argumentREnd();
      }
      const_reverse_iterator rend(void) const {
        return argumentREnd();
      }
    };
  }

  /// Specify the interface for function call statements.
  class Call {};

  namespace detail {
    class AllocateInterface : public Statement<DualExpression>,
                              public LeafStatement,
                              public boost::enable_shared_from_this<Statement<Allocate> > {
    public:
      typedef ptr<const Symbol<Type<TypeBase> > > TypePtr;

    private:
      TypePtr theType;

      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      typedef ExpressionIterator iterator;
      typedef ConstExpressionIterator const_iterator;
      typedef ReverseExpressionIterator reverse_iterator;
      typedef ConstReverseExpressionIterator const_reverse_iterator;

      template<typename E1, typename E2, typename T>
      AllocateInterface(E1 e1, E2 e2, T t)
          : Statement<DualExpression>(e1, e2), LeafStatement(), theType(t) {}

      typedef ExpressionPtr ChildPtr;
      typedef ConstExpressionPtr ConstChildPtr;

      ptr<Node<Base> > getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<const Node<Base> > getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }

      TypePtr type(void) const {
        return theType;
      }

      iterator begin(void) {
        return expressionBegin();
      }
      const_iterator begin(void) const {
        return expressionBegin();
      }
      reverse_iterator rbegin(void) {
        return expressionRBegin();
      }
      const_reverse_iterator rbegin(void) const {
        return expressionRBegin();
      }

      iterator end(void) {
        return expressionEnd();
      }
      const_iterator end(void) const {
        return expressionEnd();
      }
      reverse_iterator rend(void) {
        return expressionREnd();
      }
      const_reverse_iterator rend(void) const {
        return expressionREnd();
      }
    };
  }

  /// Specify the interface to alloc statements.
  // TODO: Add alignment information.
  class Allocate {};
}

#endif
