#ifndef mirv_Core_Filter_BackwardFlow_hpp
#define mirv_Core_Filter_BackwardFlow_hpp

#include <mirv/Filter/Flow.hpp>

namespace mirv {
  /// This flow walks backward through statements, visiting rightmost
  /// operands first.
   template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeStmtAction,
      typename AfterStmtAction,
     typename BetweenStmtAction,
     typename JoinAction,
      typename BeforeExprAction,
     typename ExprAction,
     typename AfterExprAction
     >
   class BackwardFlow
         : public StatementFlow<
      EnterAction,
      LeaveAction,
      BeforeStmtAction,
      AfterStmtAction,
      BetweenStmtAction,
     JoinAction,
      BeforeExprAction,
     ExprAction,
     AfterExprAction
     > {
     typedef Map<ptr<Expression<Base>>::type, bool>::type LabelIterateMapType;
     LabelIterateMapType iterateMap;

   public:
     BackwardFlow(const EnterAction &e,
                   const LeaveAction &l,
                   const BeforeStmtAction &bs,
                   const AfterStmtAction &as,
                  const JoinAction &j,
                  const BetweenStmtAction &bts,
                   const BeforeExprAction &be,
                  const ExprAction &ex,
                  const AfterExprAction &ae)
         : StatementFlow(e, l, bs, as, bts, j, be, ex, ae) {}

    // Allow in-place construction of actions.
    template<typename ...Args>
    BackwardFlow(Args &...args) : BaseType(args...) {}

     /// Visit block statements from the last statement to the first.
      void visit(ptr<Statement<Block> >::type stmt) {
         enter(stmt);
         for(Statement<Block>::reverse_iterator s = stmt->rbegin(),
                send = stmt->rend();
             s != send;
             ;) {
            beforeStatement(stmt, s);
            (*s)->accept(*this);
            afterStatement(stmt, s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               betweenStatement(stmt, prev, s);
            }
         }
         leave(stmt);
      }

     /// Visit IfThen statements, visiting the then-statement, then
     /// the controlling expression.
      void visit(ptr<Statement<IfThen> >::type stmt) {
         enter(stmt);

         beforeStatement(stmt, stmt->begin());
         stmt->child()->accept(*this);
         afterStatement(stmt, stmt->begin());

         this->joinPoint(stmt, stmt->child());

         beforeExpression(stmt, stmt->expressionBegin());
         this->doExpression(stmt, stmt->expressionBegin());
         afterExpression(stmt, stmt->expressionBegin());

         leave(stmt);
      }

     /// Visit IfElse statements, visiting the then-statement, then
     /// the else-statement, then the controlling expression.
      void visit(ptr<Statement<IfElse> >::type stmt) {
         enter(stmt);

         Dataflow denter(dataflow());

         Statement<IfElse>::iterator s = stmt->begin();

         beforeStatement(stmt, s);
         (*s)->accept(*this);
         afterStatement(stmt, s);

         Statement<IfElse>::iterator prev = s++;

         betweenStatement(stmt, prev, s);

         beforeStatement(stmt, s);
         (*s)->accept(*this);
         afterStatement(stmt, s);

         this->joinPoint(stmt, prev, s);

         beforeExpression(stmt, stmt->expressionBegin());
         this->doExpression(stmt, stmt->expressionBegin());
         afterExpression(stmt, stmt->expressionBegin());

         leave(stmt);
      }

     /// Visit While statements, visiting the body, then the
     /// controlling expression.
      void visit(ptr<Statement<While> >::type stmt) {
         enter(stmt);

         beforeExpression(stmt, stmt->expressionBegin());
         this->doExpression(stmt, stmt->expressionBegin());
         afterExpression(stmt, stmt->expressionBegin());

         do {
            beforeStatement(stmt, stmt->begin());
            stmt->child()->accept(*this);
            afterStatement(stmt, stmt->begin());

            beforeExpression(stmt, stmt->expressionBegin());
            this->doExpression(stmt, stmt->expressionBegin());
            afterExpression(stmt, stmt->expressionBegin());

            // Join point: Enter while and expression.
         } while (this->joinPoint(stmt, stmt->expressionBegin()));

         leave(stmt);
      }

     /// Visit DoWhile statements, visiting the controlling
     /// expression, then the body.
      void visit(ptr<Statement<DoWhile> >::type stmt) {
         enter(stmt);

         do {
            beforeStatement(stmt, stmt->begin());
            stmt->child()->accept(*this);
            afterStatement(stmt, *stmt->begin());

            beforeExpression(stmt, stmt->expressionBegin());
            this->doExpression(stmt, stmt->expressionBegin());
            afterExpression(stmt, stmt->expressionBegin());

            // Join point: Body and expression
         } while (this->joinPoint(stmt,
                                  stmt->begin(),
                                  stmt->expressionBegin()));

         leave(stmt);
      }

     /// Visit Switch statements, visiting the cases in reverse order,
     /// then the controlling expression.
      void visit(ptr<Statement<Switch> >::type stmt) {
         enter(stmt);

         for(Statement<Switch>::reverse_iterator s = stmt->rbegin(),
                send = stmt->rend();
             s != send;
             ;) {
            beforeStatement(stmt, s);
            (*s)->accept(*this);
            afterStatement(stmt, s);
            Statement<Block>::iterator prev = s;

            // Join point: Jump to label and fall-through.
            this->joinPoint(stmt, s, prev);

            if (++s != send) {
               betweenStatement(stmt, prev, s);
            }
         }

         beforeExpression(stmt, stmt->expressionBegin());
         this->doExpression(stmt, stmt->expressionBegin());
         afterExpression(stmt, stmt->expressionBegin());

         leave(stmt);
      }

     /// Visit Case statements, visiting the body, then the
     /// controlling expression.
      void visit(ptr<Statement<Case> >::type stmt) {
         enter(stmt);

         beforeStatement(stmt, stmt->begin());
         stmt->child()->accept(*this);
         afterStatement(stmt, stmt->begin());

         beforeExpression(stmt, stmt->expressionBegin());
         this->doExpression(stmt, stmt->expressionBegin());
         afterExpression(stmt, stmt->expressionBegin());

         leave(stmt);
      }

      void visit(ptr<Statement<CaseBlock> >::type stmt);

     /// Visit Before statements.
      void visit(ptr<Statement<Before> >::type stmt) {
         enter(stmt);

         LabelIterateMapType::iterator i;
         bool inserted = false;

         tie(i, inserted) =
           iterateMap.insert(std::make_pair(stmt->getExpression(), false));
         checkInvariant(inserted, "Duplicate label in iterate map");

         do {
            beforeStatement(stmt, stmt->begin());
            stmt->child()->accept(*this);
            afterStatement(stmt, stmt->begin());

            // Join point: Statement and jump.  Handled at jump.
         } while(i->second);

         iterateMap.erase(i);

         this->doLeave(stmt);
      }

     /// Visit After statements.
      void visit(ptr<Statement<After> >::type stmt) {
         enter(stmt);

         // Join point: Statement and jump.  Hanled at jump.

         beforeStatement(stmt, stmt->begin());
         stmt->child()->accept(*this);
         afterStatement(stmt, stmt->begin());

         leave(stmt);
      }

     /// Visit Goto statements.
      void visit(ptr<Statement<Goto> >::type stmt) {
         enter(stmt);

         LabelIterateMapType::iterator i =
           iterateMap.find(stmt->getExpression());

         // Join point: Jump and target.
         bool iterate = this->joinPoint(stmt, stmt->begin());

         if (i != iterateMap.end()) {
           i->second = iterate;
         }

         leave(stmt);
      }

     /// Visit Return statements.
      void visit(ptr<Statement<Return> >::type stmt) {
         enter(stmt);
         leave(stmt);
      }

     /// Visit Phi statements.
      void visit(ptr<Statement<Phi> >::type stmt) {
         this->doEnter(stmt);

         for (auto i = stmt->expressionRBegin();
              i != stmt->expressionREnd();
              ++i) {
           this->doBeforeExpression(stmt, i);
           this->doExpression(stmt, i);
           this->afterExpression(stmt, i);
         }

         this->doLeave(stmt);
      }

     /// Visit Store statements, visiting the left-hand side,
     /// then the right-hand side.
      void visit(ptr<Statement<Store> >::type stmt) {
         this->doEnter(stmt);

         for (auto i = stmt->expressionBegin();
              i != stmt->expressionEnd();
              ++i) {
           this->doBeforeExpression(stmt, i);
           this->doExpression(stmt, i);
           this->afterExpression(stmt, i);
         }

         this->doLeave(stmt);
      }

     /// Visit Call statements.
      void visit(ptr<Statement<Call> >::type stmt) {
         this->doEnter(stmt);

         for (auto i = stmt->expressionRBegin();
              i != stmt->expressionREnd();
              ++i) {
           this->doBeforeExpression(stmt, i);
           this->doExpression(stmt, i);
           this->afterExpression(stmt, i);
         }

         this->doLeave(stmt);
      }

     /// Visit Allocate statements, visiting the left-hand side,
     /// then the right-hand side.
      void visit(ptr<Statement<Allocate> >::type stmt) {
         this->doEnter(stmt);

         for (auto i = stmt->expressionBegin();
              i != stmt->expressionEnd();
              ++i) {
           this->doBeforeExpression(stmt, i);
           this->doExpression(stmt, i);
           this->afterExpression(stmt, i);
         }

         this->doLeave(stmt);
      }
   };

  /// This is a type generator to create forward flow types.
  struct BackwardFlowGenerator {
    template<
      typename EnterAction,
      typename LeaveAction,
      typename BeforeStmtAction,
      typename AfterStmtAction,
      typename BetweenStmtAction,
      typename BeforeExprAction,
      typename ExprAction,
      typename AfterExprAction
      >
    /// This is the metafunction to generate a BackwardFlow type with
    /// the given configuration.
    struct apply {
      typedef BackwardFlow<
        EnterAction,
        LeaveAction,
        BeforeStmtAction, 
        AfterStmtAction,
        BetweenStmtAction,
        JoinAction,
        BeforeExprAction,
        ExprAction,
        AfterExprAction
        > type;
    };
  };
}

#endif
