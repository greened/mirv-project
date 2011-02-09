#ifndef mirv_Core_Filter_ConstBackwardStatementFlow_hpp
#define mirv_Core_Filter_ConstBackwardStatementFlow_hpp

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
   class ConstBackwardStatementFlow
         : public ConstStatementFlow<
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
     typedef Map<ptr<Expression<Base>>::const_type, bool>::type LabelIterateMapType;
     LabelIterateMapType iterateMap;

   public:
     ConstBackwardStatementFlow(const EnterAction &e,
                                const LeaveAction &l,
                                const BeforeStmtAction &bs,
                                const AfterStmtAction &as,
                                const JoinAction &j,
                                const BetweenStmtAction &bts,
                                const BeforeExprAction &be,
                                const ExprAction &ex,
                                const AfterExprAction &ae)
         : ConstStatementFlow(e, l, bs, as, bts, j, be, ex, ae) {}

    // Allow in-place construction of actions.
    template<typename ...Args>
    ConstBackwardStatementFlow(Args &...args) : BaseType(args...) {}

     /// Visit block statements from the last statement to the first.
      void visit(ptr<Statement<Block> >::const_type stmt) {
         enter(stmt);
         for(Statement<Block>::reverse_iterator s = stmt->rbegin(),
                send = stmt->rend();
             s != send;
             ;) {
            beforeStatement(stmt, *s);
            (*s)->accept(*this);
            afterStatement(stmt, **s);
            Statement<Block>::iterator prev = s;
            if (++s != send) {
               betweenStatement(stmt, **prev, **s);
            }
         }
         leave(stmt);
      }

     /// Visit IfThen statements, visiting the then-statement, then
     /// the controlling expression.
      void visit(ptr<Statement<IfThen> >::const_type stmt) {
         enter(stmt);

         beforeStatement(stmt, *stmt->child());
         stmt->child()->accept(*this);
         afterStatement(stmt, *stmt->child());

         this->joinPoint(stmt, stmt->child());

         beforeExpression(stmt, *stmt->expression());
         this->doExpression(stmt, stmt->expression());
         afterExpression(stmt, *stmt->expression());

         leave(stmt);
      }

     /// Visit IfElse statements, visiting the then-statement, then
     /// the else-statement, then the controlling expression.
      void visit(ptr<Statement<IfElse> >::const_type stmt) {
         enter(stmt);

         Dataflow denter(dataflow());

         Statement<IfElse>::iterator s = stmt->begin();

         beforeStatement(stmt, *s);
         (*s)->accept(*this);
         afterStatement(stmt, *s);

         Statement<IfElse>::iterator prev = s++;

         betweenStatement(stmt, *prev, *s);

         beforeStatement(stmt, *s);
         (*s)->accept(*this);
         afterStatement(stmt, *s);

         this->joinPoint(stmt, *prev, *s);

         beforeExpression(stmt, *stmt->expression());
         this->doExpression(stmt, stmt->expression());
         afterExpression(stmt, *stmt->expression());

         leave(stmt);
      }

     /// Visit While statements, visiting the body, then the
     /// controlling expression.
      void visit(ptr<Statement<While> >::const_type stmt) {
         enter(stmt);

         beforeExpression(stmt, *stmt->expression());
         this->doExpression(stmt, stmt->expression());
         afterExpression(stmt, *stmt->expression());

         do {
            beforeStatement(stmt, *stmt->child());
            stmt->child()->accept(*this);
            afterStatement(stmt, *stmt->child());

            beforeExpression(stmt, *stmt->expression());
            this->doExpression(stmt, stmt->expression());
            afterExpression(stmt, *stmt->expression());

            // Join point: Enter while and expression.
         } while (this->joinPoint(stmt, stmt->expression()));

         leave(stmt);
      }

     /// Visit DoWhile statements, visiting the controlling
     /// expression, then the body.
      void visit(ptr<Statement<DoWhile> >::const_type stmt) {
         enter(stmt);

         do {
            beforeStatement(stmt, *stmt->child());
            stmt->child()->accept(*this);
            afterStatement(stmt, *stmt->child());

            beforeExpression(stmt, *stmt->expression());
            this->doExpression(stmt, stmt->expression());
            afterExpression(stmt, *stmt->expression());

            // Join point: Body and expression
         } while (this->joinPoint(stmt, stmt->child(), stmt->expression()));

         leave(stmt);
      }

     /// Visit Switch statements, visiting the cases in reverse order,
     /// then the controlling expression.
      void visit(ptr<Statement<Switch> >::const_type stmt) {
         enter(stmt);

         for(Statement<Switch>::reverse_iterator s = stmt->rbegin(),
                send = stmt->rend();
             s != send;
             ;) {
            beforeStatement(stmt, *s);
            (*s)->accept(*this);
            afterStatement(stmt, *s);
            Statement<Block>::iterator prev = s;

            // Join point: Jump to label and fall-through.
            this->joinPoint(stmt, *s, *prev);

            if (++s != send) {
               betweenStatement(stmt, **prev, **s);
            }
         }

         beforeExpression(stmt, *stmt->expression());
         this->doExpression(stmt, stmt->expression());
         afterExpression(stmt, *stmt->expression());

         leave(stmt);
      }

     /// Visit Case statements, visiting the body, then the
     /// controlling expression.
      void visit(ptr<Statement<Case> >::const_type stmt) {
         enter(stmt);

         beforeStatement(stmt, *stmt->child());
         stmt->child()->accept(*this);
         afterStatement(stmt, *stmt->child());

         beforeExpression(stmt, *stmt->expression());
         this->doExpression(stmt, stmt->expression());
         afterExpression(stmt, *stmt->expression());

         leave(stmt);
      }

      void visit(ptr<Statement<CaseBlock> >::const_type stmt);

     /// Visit Before statements.
      void visit(ptr<Statement<Before> >::const_type stmt) {
         enter(stmt);

         LabelIterateMapType::iterator i;
         bool inserted = false;

         tie(i, inserted) =
           iterateMap.insert(std::make_pair(stmt->getExpression(), false));
         checkInvariant(inserted, "Duplicate label in iterate map");

         do {
            beforeStatement(stmt, *stmt->child());
            stmt->child()->accept(*this);
            afterStatement(stmt, *stmt->child());

            // Join point: Statement and jump.  Handled at jump.
         } while(i->second);

         iterateMap.erase(i);

         this->doLeave(stmt);
      }

     /// Visit After statements.
      void visit(ptr<Statement<After> >::const_type stmt) {
         enter(stmt);

         // Join point: Statement and jump.  Hanled at jump.

         beforeStatement(stmt, *stmt->child());
         stmt->child()->accept(*this);
         afterStatement(stmt, *stmt->child());

         leave(stmt);
      }

     /// Visit Goto statements.
      void visit(ptr<Statement<Goto> >::const_type stmt) {
         enter(stmt);

         LabelIterateMapType::iterator i =
           iterateMap.find(stmt->getExpression());

         // Join point: Jump and target.
         bool iterate = this->joinPoint(stmt, stmt, stmt->child());

         if (i != iterateMap.end()) {
           i->second = iterate;
         }

         leave(stmt);
      }

     /// Visit Return statements.
      void visit(ptr<Statement<Return> >::const_type stmt) {
         enter(stmt);
         leave(stmt);
      }

     /// Visit Assignment statements, visiting the left-hand side,
     /// then the right-hand side.
      void visit(ptr<Statement<Assignment> >::const_type stmt) {
         this->doEnter(stmt);

         for (auto e = stmt->argumentRBegin();
              e != stmt->argumentREend();
              ++e) {
           this->doBeforeExpression(stmt, *e);
           this->doExpression(stmt, *e);
           this->doAfterExpression(stmt, *e);
         }

         this->doBeforeExpression(stmt, stmt->function());
         this->doExpression(stmt, stmt->function());
         this->doAfterExpression(stmt, stmt->function());

         this->doLeave(stmt);
      }

     /// Visit Call statements.
      void visit(ptr<Statement<Call> >::const_type stmt) {
         this->doEnter(stmt);


         this->doBeforeExpression(stmt, stmt->getLeftExpression());
         this->doExpression(stmt, stmt->getLeftExpression());
         this->afterExpression(stmt, stmt->getLeftExpression());

         this->doBeforeExpression(stmt, stmt->getRightExpression());
         this->doExpression(stmt, stmt->getRightExpression());
         this->afterExpression(stmt, stmt->getRightExpression());

         this->doLeave(stmt);
      }
   };

  /// This is a type generator to create forward flow types.
  struct ConstBackwardStatementFlowGenerator {
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
    struct apply {
      typedef ConstBackwardStatementFlow<
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