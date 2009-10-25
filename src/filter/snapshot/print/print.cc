#include "print.hh"

namespace mirv {
  const int PrintFilter::IndentFactor = 3;

   void PrintFilter::EnterAction::visit(Statement<Block> &stmt)
   {
      out << "{\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(Statement<IfThen> &stmt)
    {
      out << indent(ind) << "ifThen\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(Statement<IfElse> &stmt)
    {
       out << indent(ind) << "ifElse\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(Statement<While> &stmt)
    {
       out << indent(ind) << "while\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(Statement<DoWhile> &stmt)
    {
       out << indent(ind) << "doWhile\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(Statement<Switch> &stmt)
    {
       out << indent(ind) << "switch\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterAction::visit(Statement<Case> &stmt)
    {
       out << indent(ind) << "case";
    }

    void PrintFilter::EnterAction::visit(Statement<Before> &stmt)
    {
       out << indent(ind) << "before ";
    }

    void PrintFilter::EnterAction::visit(Statement<After> &stmt)
    {
       out << indent(ind) << "after ";
    }

    void PrintFilter::EnterAction::visit(Statement<Goto> &stmt)
    {
       out << indent(ind) << "goto ";
    }

    void PrintFilter::EnterAction::visit(Statement<Return> &stmt)
    {
       out << indent(ind) << "return\n";
    }

   void PrintFilter::LeaveAction::visit(Statement<Block> &stmt) {
     ind -= IndentFactor;
     out << indent(ind) << "}";
   }

    void PrintFilter::EnterExprAction::visit(Expression<Add> &expr)
    {
       out << indent(ind) << "+\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(Expression<Subtract> &expr)
    {
       out << indent(ind) << "-\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(Expression<Multiply> &expr)
    {
       out << indent(ind) << "*\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(Expression<Divide> &expr)
    {
       out << indent(ind) << "/\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(Expression<Modulus> &expr)
    {
       out << indent(ind) << "%\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(Expression<Negate> &expr)
    {
       out << indent(ind) << "-\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(Expression<LogicalAnd> &expr)
    {
       out << indent(ind) << "&&\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(Expression<LogicalOr> &expr)
    {
       out << indent(ind) << "||\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(Expression<LogicalNot> &expr)
    {
       out << indent(ind) << "!\n";
       ind += IndentFactor;
    }

    void PrintFilter::EnterExprAction::visit(Expression<BitwiseAnd> &expr)
    {
      out << indent(ind) << "&\n";
      ind += IndentFactor;
   }

   void PrintFilter::EnterExprAction::visit(Expression<BitwiseOr> &expr)
   {
      out << indent(ind) << "|\n";
      ind += IndentFactor;
   }

   void PrintFilter::EnterExprAction::visit(Expression<BitwiseComplement> &expr)
   {
      out << indent(ind) << "~\n";
      ind += IndentFactor;
   }

   void PrintFilter::operator()(BaseNode &node)
   {
     if (BaseStatement *s = dynamic_cast<BaseStatement *>(&node)) {
       ptr<StatementVisitor>::type flow =
	 make_forward_flow(EnterAction(out, ind),
			   LeaveAction(out, ind),
			   NullAction(),
			   AfterAction(out, ind),
			   NullAction(),
			   NullAction(),
			   NullAction(),
			   PrintExpressionFlow(EnterExprAction(out, ind),
					       LeaveExprAction(out, ind),
					       AfterExprAction(out, ind)));
       s->accept(*flow);
     }
     else {
     }
   }
}
