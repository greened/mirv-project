#ifndef mirv_Core_IR_Control_hpp
#define mirv_Core_IR_Control_hpp

#include <mirv/Core/IR/ControlFwd.hpp>
#include <mirv/Core/Builder/Make.hpp>
#include <mirv/Core/IR/Statement.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/mpl/vector.hpp>

namespace mirv {
  namespace detail {
    class BlockInterface : public InnerStatement,
                           public boost::enable_shared_from_this<Statement<Block> > {
    private:
      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      BlockInterface(void) : InnerStatement() {}
      template<typename A1>
      BlockInterface(A1 a1) : InnerStatement(a1)  {}
      template<typename A1, typename A2>
      BlockInterface(A1 a1, A2 a2) : InnerStatement(a1, a2)  {}
      template<typename A1, typename A2, typename A3>
      BlockInterface(A1 a1, A2 a2, A3 a3) : InnerStatement(a1, a2, a3)  {}
      template<typename A1, typename A2, typename A3, typename A4>
      BlockInterface(A1 a1, A2 a2, A3 a3, A4 a4) : InnerStatement(a1, a2, a3, a4)  {}

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(shared_from_this());
      }
    };
  }

  /// Specify the Interface for block/sequence statements.
  class Block {};

  namespace detail {
    class SingleBlockInterface : public InnerStatement {
    public:
      template<typename A1>
      // If this isn't a block statement, make it one.
      SingleBlockInterface(A1 a1) : 
          InnerStatement(dyn_cast<Statement<Block> >(a1) ?
                         a1 : mirv::make<Statement<Block> >(a1))  {}
 
      typedef InnerStatement::ChildPtr ChildPtr;
      typedef InnerStatement::ConstChildPtr ConstChildPtr;
         
      void setChildStatement(ChildPtr s) {
        if (!dyn_cast<Statement<Block> >(s)) {
          s = mirv::make<Statement<Block> >(s);
        }
        if (this->empty()) {
          push_back(s);
        }
        else {
          *--this->end() = s;
        }
      }

      ChildPtr getChildStatement(void) {
        checkInvariant(!this->empty(), 
                       "Attempt to get statement from empty block");
	  
        return(this->front());
      }
      ConstChildPtr getChildStatement(void) const {
        checkInvariant(!this->empty(), 
                       "Attempt to get statement from empty block");
	  
        return(this->front());
      }
    };
  }

  /// This is a statement with only one child statement.  The child
  /// should be a block statement if more than one statement needs to
  /// be under this control point.
  class SingleBlock {};

  namespace detail {
    class DualBlockInterface : public InnerStatement {
      // Protected because these are probably bad names for subclasses
    protected:
      // If these are not blocks, make them so.
      template<typename A1, typename A2>
      DualBlockInterface(A1 a1, A2 a2) :
	  InnerStatement(dyn_cast<Statement<Block> >(a1)?
                         a1 : mirv::make<Statement<Block> >(a1),
                         dyn_cast<Statement<Block> >(a2) ?
                            a2 : mirv::make<Statement<Block> >(a2))  {}

      typedef InnerStatement::ChildPtr ChildPtr;
      typedef InnerStatement::ConstChildPtr ConstChildPtr;
         
      void setLeftChildStatement(ChildPtr s) {
        if (!dyn_cast<Statement<Block> >(s)) {
          s = mirv::make<Statement<Block> >(s);
        }
        if (this->empty()) {
          push_back(s);
        }
        else {
          *this->begin() = s;
        }
      }

      void setRightChildStatement(ChildPtr s) {
        if (!dyn_cast<Statement<Block> >(s)) {
          s = mirv::make<Statement<Block> >(s);
        }
        if (this->empty()) {
          push_back(ChildPtr());  // Placeholder for left operand
          push_back(s);
        }
        else {
          *--this->end() = s;
        }
      }

      ChildPtr getLeftChildStatement(void) {      
        checkInvariant(!this->empty(),
                       "Attempt to get statement from empty block");
        return(this->front());
      };
         
      ConstChildPtr getLeftChildStatement(void) const {
        checkInvariant(!this->empty(),
                       "Attempt to get statement from empty block");
        return(this->front());
      };
         
      ChildPtr getRightChildStatement(void) {      
        checkInvariant(this->size() > 1 && this->back(),
                       "Attempt to get missing statement from block");
        return(this->back());
      };
         
      ConstChildPtr getRightChildStatement(void) const {
        checkInvariant(this->size() > 1 && this->back(),
                       "Attempt to get missing statement from block");
        return(this->back());
      };
    };
  }

  /// This is a statement with two child statements.  The children
  /// should be block statements if more than one statement needs to
  /// be under this control point.
  class DualBlock {};

  namespace detail {
    class SingleConditionInterface 
        : public Statement<SingleExpression> {
      typedef Statement<SingleExpression>::ExpressionPtr ExpressionPtr;
      typedef Statement<SingleExpression>::ConstExpressionPtr 
      ConstExpressionPtr;

    public:
      template<typename A1>
      SingleConditionInterface(A1 a1) : Statement<SingleExpression>(a1)  {}

      void setCondition(ExpressionPtr e) {
        setExpression(e);
      };

      ExpressionPtr getCondition(void) {
        return(this->getExpression());
      };

      ConstExpressionPtr getCondition(void) const {
        return(this->getExpression());
      };
    };
  }

  /// This is a statement with a single controlling condition
  class SingleCondition {};

  namespace detail {
    class IfThenInterface : public Statement<SingleCondition>,
                            public Statement<SingleBlock>,
                            public boost::enable_shared_from_this<Statement<IfThen> > {
      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      template<typename S, typename E>
      IfThenInterface(E e, S s) : Statement<SingleCondition>(e),
                                    Statement<SingleBlock>(s) {}

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base>>(shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base>>(shared_from_this());
      }

       // We need this to be the final overriders for
       // Visitable::accept functions.
       virtual void accept(StatementVisitor &) {
         error("IfThen::Interface::accept called!");
       }
       virtual void accept(ConstStatementVisitor &) const {
         error("IfThen::Interface::accept called!");
       }
    };
  }

  /// Specify the if-then statement Interface.
  class IfThen {};

  namespace detail {
    class IfElseInterface : public Statement<SingleCondition>,
                            public Statement<DualBlock>,
                            public boost::enable_shared_from_this<Statement<IfElse> > {
      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      template<typename S1, typename S2, typename E>
      IfElseInterface(E e, S1 s1, S2 s2) : Statement<SingleCondition>(e),
                                             Statement<DualBlock>(s1, s2) {}
      
      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }

       // We need this to be the final overriders for
       // Visitable::accept functions.
       virtual void accept(StatementVisitor &) {
         error("IfElse::Interface::accept called!");
       }
       virtual void accept(ConstStatementVisitor &) const {
         error("IfElse::Interface::accept called!");
       }
    };
  }

  /// Specify the if-then-else statement Interface.
  class IfElse {};

  namespace detail {
    class WhileInterface : public Statement<SingleCondition>,
                           public Statement<SingleBlock>,
                           public boost::enable_shared_from_this<Statement<While> > {
      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      template<typename S1, typename E>
      WhileInterface(E e, S1 s1) : Statement<SingleCondition>(e),
                                     Statement<SingleBlock>(s1) {}

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }

       // We need this to be the final overriders for
       // Visitable::accept functions.
       virtual void accept(StatementVisitor &) {
         error("While::Interface::accept called!");
       }
       virtual void accept(ConstStatementVisitor &) const {
         error("While::Interface::accept called!");
       }
    };
  }

  /// Specify the while statement Interface.
  class While {};

  namespace detail {
    class DoWhileInterface : public Statement<SingleCondition>,
                             public Statement<SingleBlock>,
                             public boost::enable_shared_from_this<Statement<DoWhile> > {
      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      template<typename S1, typename E>
      DoWhileInterface(E e, S1 s1) : Statement<SingleCondition>(e),
                                       Statement<SingleBlock>(s1) {}

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }

       // We need this to be the final overriders for
       // Visitable::accept functions.
       virtual void accept(StatementVisitor &) {
         error("DoWhile::Interface::accept called!");
       }
       virtual void accept(ConstStatementVisitor &) const {
         error("DoWhile::Interface::accept called!");
       }
    };
  }

  /// Specify the do-while statement Interface.
  class DoWhile {};

  namespace detail {
    class CaseInterface : public Statement<SingleCondition>,
                          public Statement<SingleBlock>,
                          public boost::enable_shared_from_this<Statement<Case> > {
      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      template<typename S1, typename E>
      CaseInterface(E e, S1 s1) : Statement<SingleCondition>(e),
                                    Statement<SingleBlock>(s1) {}

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }

       // We need this to be the final overriders for
       // Visitable::accept functions.
       virtual void accept(StatementVisitor &) {
         error("Case::Interface::accept called!");
       }
       virtual void accept(ConstStatementVisitor &) const {
         error("Case::Interface::accept called!");
       }
    };
  }

  /// Specify the case statement Interface.
  class Case {};
   
  namespace detail {
    class CaseBlockInterface : public virtual Statement<SingleBlock> {
      typedef Statement<SingleBlock> BaseType;
      typedef BaseType::ChildPtr ChildPtr;

    protected:
      void setParents(void);

    public:
      template<typename S1>
      CaseBlockInterface(S1 s1) : BaseType(s1) {}

      // Override base methods
      void push_back(ChildPtr c) {
        // check_invariant(safe_cast<Statement<Case> >(c),
        //                  "Attempt to insert non-case statement in case block");
        BaseType::push_back(c);
      };
    };
  }

  /// This is a list of case statements.  It is the statement type
  /// that appears under a switch statement.
  class CaseBlock {};

  namespace detail {
    class SwitchInterface : public Statement<SingleCondition>,
                            public Statement<SingleBlock>,
                            public boost::enable_shared_from_this<Statement<Switch> > {
      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      template<typename S1, typename E>
      SwitchInterface(E e, S1 s1) : Statement<SingleCondition>(e),
                                      Statement<SingleBlock>(s1) {}

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }

       // We need this to be the final overriders for
       // Visitable::accept functions.
       virtual void accept(StatementVisitor &) {
         error("Switch::Interface::accept called!");
       }
       virtual void accept(ConstStatementVisitor &) const {
         error("Switch::Interface::accept called!");
       }
    };
  }

  /// Specify the switch statement Interface.
  class Switch {};

  namespace detail {
    class SingleLabelInterface 
        : public Statement<SingleExpression> {
      typedef Statement<SingleExpression> BaseType;

      typedef BaseType::ExpressionPtr ExpressionPtr;
      typedef BaseType::ConstExpressionPtr 
      ConstExpressionPtr;

    public:
      template<typename E>
      SingleLabelInterface(E e) : BaseType(e) {}

      void setLabel(ExpressionPtr e) {
        // check_invariant(safe_cast<Expression<Label> >(e),
        //                  "Attempt to set non-label as label");
            
        setExpression(e);
      };

      ExpressionPtr getLabel(void) {
        return(this->getExpression());
      };

      ConstExpressionPtr getLabel(void) const {
        return this->getExpression();
      };
    };
  }

  /// This is a statement with a single label
  class SingleLabel {};

  namespace detail {
    class BeforeInterface : public Statement<SingleLabel>,
                            public Statement<SingleBlock>,
                            public boost::enable_shared_from_this<Statement<Before> > {
      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      template<typename S1, typename E>
      BeforeInterface(E e, S1 s1) : Statement<SingleLabel>(e),
                                      Statement<SingleBlock>(s1) {}

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }

       // We need this to be the final overriders for
       // Visitable::accept functions.
       virtual void accept(StatementVisitor &) {
         error("Before::Interface::accept called!");
       }
       virtual void accept(ConstStatementVisitor &) const {
         error("Before::Interface::accept called!");
       }
    };
  }

  /// A before statement specifies a block statement with an entry
  /// label.  A goto-dest statement may transfer control to this
  /// label.  This provides a structured way to return to the top of a
  /// deeply nested set of loops.
  class Before {};

  namespace detail {
    class AfterInterface : public Statement<SingleLabel>,
                           public Statement<SingleBlock>,
                           public boost::enable_shared_from_this<Statement<After> > {
      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      template<typename S1, typename E>
      AfterInterface(E e, S1 s1) : Statement<SingleLabel>(e),
                                     Statement<SingleBlock>(s1) {}

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }

       // We need this to be the final overriders for
       // Visitable::accept functions.
       virtual void accept(StatementVisitor &) {
         error("After::Interface::accept called!");
       }
       virtual void accept(ConstStatementVisitor &) const {
         error("After::Interface::accept called!");
       }
    };
  }

  /// An after statement specifies a block with a label at the end.  A
  /// goto-dest statement may transfer control to this label.  This
  /// provides a structured way to implement break- and continue-type
  /// statements.
  class After {};

  namespace detail {
    class GotoInterface : public Statement<SingleLabel>,
                          public LeafStatement,
                          public boost::enable_shared_from_this<Statement<Goto> > {
      Statement<Base> *cloneImpl(void);

    protected:
      void setParents(void);

    public:
      template<typename E>
      GotoInterface(E e) : Statement<SingleLabel>(e),
                             LeafStatement() {}

      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }
    };
  }

  /// This is an arbitrary goto statement.  It should not appear in
  /// normalized IR but is necessary to handle translation from
  /// higher level languages.  A filter will eliminate gotos and
  /// restructure the IR to be fully structured.
  class Goto {};

  namespace detail {
    class ReturnInterface : public LeafStatement,
                            public boost::enable_shared_from_this<Statement<Return> > {
      Statement<Base> *cloneImpl(void);

    public:
      ptr<Node<Base> >::type getSharedHandle(void) {
        return fast_cast<Node<Base> >(shared_from_this());
      }
      ptr<Node<Base> >::const_type getSharedHandle(void) const {
        return fast_cast<const Node<Base> >(shared_from_this());
      }
    };
  }

  /// A return does not return an expression.  Instead, assignment to
  /// a special location determines the return value.  This separates
  /// changes in data state from changes in control state.
  class Return {};
}

#endif
