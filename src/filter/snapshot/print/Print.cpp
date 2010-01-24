#include <mirv/filter/snapshot/print/Print.hpp>

#include <mirv/core/ir/Symbol.hpp>
#include <mirv/core/ir/Variable.hpp>
#include <mirv/core/ir/Expression.hpp>
#include <mirv/core/ir/Arithmetic.hpp>
#include <mirv/core/ir/Logical.hpp>
#include <mirv/core/ir/Bitwise.hpp>
#include <mirv/core/ir/Reference.hpp>
#include <mirv/core/ir/Statement.hpp>
#include <mirv/core/ir/Control.hpp>
#include <mirv/core/ir/Mutating.hpp>
#include <mirv/core/util/Cast.hpp>

namespace mirv {
  const int PrintFilter::IndentFactor = 3;

  void PrintFilter::EnterDeclSymbolAction::visit(ptr<Symbol<Module> >::type sym)
  {
    JustLeft = false;
    out << indent(ind) << "mdef " << sym->name() << " {\n";
    ind += IndentFactor;
  }

  void PrintFilter::EnterDeclSymbolAction::visit(ptr<Symbol<Function> >::type sym)
  {
    // Just declarations.
    JustLeft = false;
    out << indent(ind) << "fdecl " << sym->name();
  }

  void PrintFilter::EnterDeclSymbolAction::visit(ptr<Symbol<Variable> >::type sym)
  {
    JustLeft = false;
    out << indent(ind) << "vdecl " << sym->name() << " "
	<< sym->type()->name();
  }

  void PrintFilter::EnterDeclSymbolAction::visit(ptr<Symbol<Type<Integral> > >::type sym)
  {
    JustLeft = false;
    out << indent(ind) << "tsdecl " << sym->name() << " integral "
	<< sym->bitsize();
  }

  void PrintFilter::EnterDeclSymbolAction::visit(ptr<Symbol<Type<Floating> > >::type sym)
  {
    JustLeft = false;
    out << indent(ind) << "tsdecl " << sym->name() << " floating "
	<< sym->bitsize();
  }

  void PrintFilter::EnterDeclSymbolAction::visit(ptr<Symbol<Type<Array> > >::type sym)
  {
    JustLeft = false;
    out << indent(ind) << "tadecl " << sym->name()
	<< sym->getElementType()->name();
    for(Symbol<Array>::ConstDimensionIterator d = sym->dimensionBegin(),
	  dend = sym->dimensionEnd();
	d != dend;
	++d) {
      out << "[" << *d << "]";
    }
  }

  void PrintFilter::EnterDeclSymbolAction::visit(ptr<Symbol<Type<Pointer> > >::type sym)
  {
    JustLeft = false;
    out << indent(ind) << "tpdecl " << sym->name()
	<< sym->getBaseType()->name();
  }

  void PrintFilter::EnterDeclSymbolAction::visit(ptr<Symbol<Type<FunctionType> > >::type sym)
  {
    JustLeft = false;
    out << indent(ind) << "tfdecl " << sym->name();
    if (!sym->getReturnType() || sym->parameterEmpty()) {
      return;
    }
    out << "{\n";
    out << indent(ind+IndentFactor) << "rtype " << sym->getReturnType()->name() << "\n";
    for (Symbol<Type<FunctionType> >::iterator p =
	   sym->parameterBegin(), pend = sym->parameterEnd();
	 p != pend;
	 ++p) {
      out << indent(ind+IndentFactor) << "ptype " << (*p)->name() << "\n";
    }
    out << "}";
  }

  void PrintFilter::EnterDefSymbolAction::visit(ptr<Symbol<Function> >::type sym)
  {
    JustLeft = false;
    out << indent(ind) << "fdef " << sym->name() << " {\n";
    ind += IndentFactor;
  }

  void PrintFilter::EnterDefSymbolAction::visit(ptr<Symbol<Variable> >::type sym)
  {
    JustLeft = false;
    out << indent(ind) << "vdecl " << sym->name() << " "
	<< sym->type()->name();
  }

   void PrintFilter::LeaveDefSymbolAction::visit(ptr<Symbol<Module> >::type sym) {
     ind -= IndentFactor;
     if (!JustLeft) {
       out << "\n";
     }
     out << indent(ind) << "}\n";
     JustLeft = true;
   }

   void PrintFilter::LeaveDefSymbolAction::visit(ptr<Symbol<Function> >::type sym) {
     ind -= IndentFactor;
     if (!JustLeft) {
       out << "\n";
     }
     out << indent(ind) << "}\n";
     JustLeft = true;
   }

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
     out << indent(ind) << "vref " << expr->getSymbol()->name();
   }

  void PrintFilter::operator()(ptr<Node<Base> >::type node)
   {
     if (ptr<Symbol<Module> >::type s = boost::dynamic_pointer_cast<Symbol<Module> >(node)) {
       JustLeft = false;
       ptr<SymbolVisitor>::type declflow(new PrintDeclSymbolFlow(EnterDeclSymbolAction(out, ind, JustLeft),
								 LeaveDeclSymbolAction(out, ind, JustLeft)));
       s->accept(*declflow);
       ptr<SymbolVisitor>::type defflow(new PrintDefSymbolFlow(EnterDefSymbolAction(out, ind, JustLeft),
							       LeaveDefSymbolAction(out, ind, JustLeft),
							       PrintFlow(EnterAction(out, ind, JustLeft),
									 LeaveAction(out, ind, JustLeft),
									 PrintExpressionFlow(EnterExprAction(out, ind, JustLeft),
											     LeaveExprAction(out, ind, JustLeft)))));
       s->accept(*defflow);
     }
     else if (ptr<Symbol<Base> >::type s = boost::dynamic_pointer_cast<Symbol<Base> >(node)) {
       JustLeft = false;
       ptr<SymbolVisitor>::type defflow(makeSymbolFlow(EnterDefSymbolAction(out, ind, JustLeft),
						       LeaveDefSymbolAction(out, ind, JustLeft),
						       NullAction(),
						       NullAction(),
						       NullAction(),
						       PrintFlow(EnterAction(out, ind, JustLeft),
								 LeaveAction(out, ind, JustLeft),
								 PrintExpressionFlow(EnterExprAction(out, ind, JustLeft),
										     LeaveExprAction(out, ind, JustLeft)))));
       s->accept(*defflow);
     }
     else if (ptr<Statement<Base> >::type s = boost::dynamic_pointer_cast<Statement<Base> >(node)) {
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
