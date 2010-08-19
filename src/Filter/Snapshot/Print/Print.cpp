#include <mirv/Filter/Snapshot/Print/Print.hpp>

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
  const int PrintFilter::IndentFactor = 3;

  void PrintFilter::EnterDeclSymbolVisitor::visit(ptr<Symbol<Module> >::type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "mdef " << sym->name() << " {\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterDeclSymbolVisitor::visit(ptr<Symbol<Function> >::type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    // Just declarations.
    out << indent(ind) << "fdecl " << sym->name();
  }

  void PrintFilter::EnterDeclSymbolVisitor::visit(ptr<Symbol<Variable> >::type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "vdecl " << sym->name() << " "
      << sym->type()->name();
  }

  void PrintFilter::EnterDeclSymbolVisitor::visit(ptr<Symbol<Type<TypeBase> > >::type sym)
  {
    // This is a type that doesn't need a declaration.  So tell the
    // printer not to print a newline after it.
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::EnterDeclSymbolVisitor::visit(ptr<Symbol<Type<StructType> > >::type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "tdecl " << sym->name();
    out << " {\n";
    for (Symbol<Type<StructType> >::iterator p =
	   sym->begin(), pend = sym->end();
	 p != pend;
	 /* NULL */) {
      out << indent(ind+IndentFactor) << (*p)->name();
      if (++p != pend) {
        out << ',';
      }
      out << '\n';
    }
    out << indent(ind) << "}";
  }

  /// Print the final newline after each symbol declaration.
  void PrintFilter::LeaveDeclSymbolVisitor::visit(ptr<Symbol<Base> >::type)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveDeclSymbolVisitor::visit(ptr<Symbol<Type<TypeBase> > >::type)
  {
    // Don't do anything since we didn't print anything for this
    // type.
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

      /// Print the final newline after struct types.
  void PrintFilter::LeaveDeclSymbolVisitor::visit(ptr<Symbol<Type<StructType> > >::type)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::EnterDefSymbolVisitor::visit(ptr<Symbol<Module> >::type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterDefSymbolVisitor::visit(ptr<Symbol<Function> >::type sym)
  {
    if (!sym->statementEmpty()) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "fdef " << sym->name() << " {\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }
  }

  void PrintFilter::EnterDefSymbolVisitor::visit(ptr<Symbol<Variable> >::type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "vdecl " << sym->name() << " "
      << sym->type()->name();
  }

  void PrintFilter::LeaveDefSymbolVisitor::visit(ptr<Symbol<Variable> >::type sym)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
           || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveDefSymbolVisitor::visit(ptr<Symbol<Module> >::type sym) {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "}\n";
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveDefSymbolVisitor::visit(ptr<Symbol<Function> >::type sym) {
    if (!sym->statementEmpty()) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      if (   !attributeManager.setLastSynthesizedAttribute()
          || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << "\n";
      }
      out << indent(ind) << "}\n";
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Block> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "{\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<IfThen> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "ifThen\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<IfElse> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "ifElse\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<While> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "while\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<DoWhile> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "doWhile\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Switch> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "switch\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Case> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "case";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<CaseBlock> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "caseblock";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Before> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "before ";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<After> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "after ";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Goto> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "goto ";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Return> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "return\n";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Assignment> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "assign\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Call> >::type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "call\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::LeaveStatementVisitor::visit(ptr<Statement<Base> >::type stmt)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveStatementVisitor::visit(ptr<Statement<Block> >::type stmt) {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      out << "\n";
    }
    out << indent(ind) << "}\n";
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveStatementVisitor::visit(ptr<Statement<Return> >::type stmt) {
    Stream &out = attributeManager.getInheritedAttribute().out();

    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      out << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Add> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "+\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Subtract> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "-\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Multiply> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "*\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Divide> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "/\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Modulus> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "%\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Negate> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "neg\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "&&\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalOr> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "||\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalNot> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "!\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "&\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "|\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "~\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LessThan> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "<\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "<=\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Equal> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "==\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<NotEqual> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "!=\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<GreaterThan> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << ">\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << ">=\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "vref " << expr->getSymbol()->name();
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Function> > >::type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "fref " << expr->getSymbol()->name();
  }

  void PrintFilter::LeaveExpressionVisitor::visit(ptr<Expression<Base> >::type expr)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveExpressionVisitor::visit(ptr<InnerExpression>::type expr)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::PrintDefSymbolFlow::visit(ptr<Symbol<Module>>::type sym) {
    // We only want to visit functions here since we already
    // declared module-level types and variables.
    this->doEnter(sym);
    // Visit functions
    for(Symbol<Module>::FunctionIterator f = sym->functionBegin(),
          fend = sym->functionEnd();
        f != fend;
        /* NULL */) {
      if (!(*f)->statementEmpty()) {
        this->doBefore(sym, *f);
        (*f)->accept(*this);
        this->doAfter(sym, *f);
      }
      Symbol<Module>::FunctionIterator prev = f;
      if (++f != fend) {
        if (!(*prev)->statementEmpty() && !(*f)->statementEmpty()) {
          this->doBetween(sym, *prev, *f);
        }
      }
    }
    this->doLeave(sym);
  }

  void PrintFilter::operator()(ptr<Node<Base> >::type node)
  {
    if (ptr<Symbol<Module> >::type s =
        boost::dynamic_pointer_cast<Symbol<Module> >(node)) {
      ptr<SymbolVisitor>::type declflow(new PrintDeclSymbolFlow(out));
      s->accept(*declflow);
      ptr<SymbolVisitor>::type defflow(new PrintDefSymbolFlow(out));
      s->accept(*defflow);
    }
    else if (ptr<Symbol<Base> >::type s =
             boost::dynamic_pointer_cast<Symbol<Base> >(node)) {
      ptr<SymbolVisitor>::type defflow(new PrintDefSymbolFlow(out));
      s->accept(*defflow);
    }
    else if (ptr<Statement<Base> >::type s =
             boost::dynamic_pointer_cast<Statement<Base> >(node)) {
      ptr<StatementVisitor>::type flow(new PrintFlow(out));
      s->accept(*flow);
    }
    else if (ptr<Expression<Base> >::type e =
             boost::dynamic_pointer_cast<Expression<Base> >(node)) {
      ptr<ExpressionVisitor>::type flow(new PrintExpressionFlow(out));
      e->accept(*flow);
    }
  }
}
