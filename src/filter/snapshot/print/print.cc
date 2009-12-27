 #include "print.hh"

#include <mirv/core/ir/symbol.hh>
#include <mirv/core/ir/variable.hh>
#include <mirv/core/ir/expression.hh>
#include <mirv/core/ir/arithmetic.hh>
#include <mirv/core/ir/logical.hh>
#include <mirv/core/ir/bitwise.hh>
#include <mirv/core/ir/reference.hh>
#include <mirv/core/ir/statement.hh>
#include <mirv/core/ir/control.hh>
#include <mirv/core/ir/mutating.hh>
#include <mirv/core/util/cast.hh>

namespace mirv {
  const int PrintFilter::IndentFactor = 3;

   void PrintFilter::EnterAction::visit(ptr<Statement<Block> >::type stmt)
   {
     JustLeft = false;
     out << indent(ind) << "{\n";
     ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<IfThen> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "ifThen\n";
      ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<IfElse> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "ifElse\n";
      ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<While> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "while\n";
      ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<DoWhile> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "doWhile\n";
      ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<Switch> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "switch\n";
      ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<Case> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "case";
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<CaseBlock> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "caseblock";
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<Before> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "before ";
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<After> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "after ";
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<Goto> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "goto ";
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<Return> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "return\n";
    }

    void PrintFilter::EnterAction::visit(ptr<Statement<Assignment> >::type stmt)
    {
      JustLeft = false;
      out << indent(ind) << "assign\n";
      ind += IndentFactor;
    }

   void PrintFilter::LeaveAction::visit(ptr<Statement<Block> >::type stmt) {
     ind -= IndentFactor;
     if (!JustLeft) {
       out << "\n";
     }
     out << indent(ind) << "}\n";
     JustLeft = true;
   }

  void PrintFilter::LeaveAction::visit(ptr<Statement<Return> >::type stmt) {
    if (!JustLeft) {
      out << "\n";
    }
    JustLeft = true;
  }

    void PrintFilter::EnterExprAction::visit(ptr<Expression<Add> >::type expr)
    {
      JustLeft = false;
       out << indent(ind) << "+\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(ptr<Expression<Subtract> >::type expr)
    {
      JustLeft = false;
       out << indent(ind) << "-\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(ptr<Expression<Multiply> >::type expr)
    {
      JustLeft = false;
       out << indent(ind) << "*\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(ptr<Expression<Divide> >::type expr)
    {
      JustLeft = false;
       out << indent(ind) << "/\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(ptr<Expression<Modulus> >::type expr)
    {
      JustLeft = false;
       out << indent(ind) << "%\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(ptr<Expression<Negate> >::type expr)
    {
      JustLeft = false;
       out << indent(ind) << "neg\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(ptr<Expression<LogicalAnd> >::type expr)
    {
      JustLeft = false;
       out << indent(ind) << "&&\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(ptr<Expression<LogicalOr> >::type expr)
    {
      JustLeft = false;
       out << indent(ind) << "||\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(ptr<Expression<LogicalNot> >::type expr)
    {
      JustLeft = false;
       out << indent(ind) << "!\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(ptr<Expression<BitwiseAnd> >::type expr)
    {
      JustLeft = false;
      out << indent(ind) << "&\n";
      ind += IndentFactor;
   }

   void PrintFilter::EnterExprAction::visit(ptr<Expression<BitwiseOr> >::type expr)
   {
      JustLeft = false;
      out << indent(ind) << "|\n";
      ind += IndentFactor;
   }

   void PrintFilter::EnterExprAction::visit(ptr<Expression<BitwiseComplement> >::type expr)
   {
      JustLeft = false;
      out << indent(ind) << "~\n";
      ind += IndentFactor;
   }

   void PrintFilter::EnterExprAction::visit(ptr<Expression<LessThan> >::type expr)
   {
      JustLeft = false;
      out << indent(ind) << "<\n";
      ind += IndentFactor;
   }

   void PrintFilter::EnterExprAction::visit(ptr<Expression<LessThanOrEqual> >::type expr)
   {
      JustLeft = false;
      out << indent(ind) << "<=\n";
      ind += IndentFactor;
   }

   void PrintFilter::EnterExprAction::visit(ptr<Expression<Equal> >::type expr)
   {
      JustLeft = false;
      out << indent(ind) << "==\n";
      ind += IndentFactor;
   }

   void PrintFilter::EnterExprAction::visit(ptr<Expression<NotEqual> >::type expr)
   {
      JustLeft = false;
      out << indent(ind) << "!=\n";
      ind += IndentFactor;
   }

   void PrintFilter::EnterExprAction::visit(ptr<Expression<GreaterThan> >::type expr)
   {
      JustLeft = false;
      out << indent(ind) << ">\n";
      ind += IndentFactor;
   }

   void PrintFilter::EnterExprAction::visit(ptr<Expression<GreaterThanOrEqual> >::type expr)
   {
      JustLeft = false;
      out << indent(ind) << ">=\n";
      ind += IndentFactor;
   }

  void PrintFilter::EnterExprAction::visit(ptr<Expression<Reference<Variable> > >::type expr)
   {
     JustLeft = false;
     out << indent(ind) << "vref " << expr->get_symbol()->name();
   }

  void PrintFilter::operator()(ptr<Node<Base> >::type node)
   {
     //if (ptr<Statement<Base> >::type s = dyn_cast<Statement<Base> >(node)) {
     if (ptr<Statement<Base> >::type s = boost::dynamic_pointer_cast<Statement<Base> >(node)) {
       JustLeft = false;
       ptr<StatementVisitor>::type flow(new PrintFlow(EnterAction(out, ind, JustLeft),
						      LeaveAction(out, ind, JustLeft),
						      PrintExpressionFlow(EnterExprAction(out, ind, JustLeft),
									  LeaveExprAction(out, ind, JustLeft))));
       s->accept(*flow);
     }
     else if (ptr<Expression<Base> >::type e = boost::dynamic_pointer_cast<Expression<Base> >(node)) {
       JustLeft = false;
       ptr<ExpressionVisitor>::type flow(new PrintExpressionFlow(EnterExprAction(out, ind, JustLeft),
								 LeaveExprAction(out, ind, JustLeft)));
       e->accept(*flow);
     }
   }
}
