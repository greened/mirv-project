#include <mirv/Filter/Snapshot/Print/Print.hpp>

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
  const int PrintFilter::IndentFactor = 3;

  void PrintFilter::EnterDeclSymbolVisitor::visit(ptr<Symbol<Module> >::const_type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "mdef " << sym->name() << " {\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterDeclSymbolVisitor::visit(ptr<Symbol<Function> >::const_type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    // Just declarations.
    out << indent(ind) << "fdecl " << sym->name() << ' '
      << sym->type()->name();
  }

  void PrintFilter::EnterDeclSymbolVisitor::visit(ptr<Symbol<Variable> >::const_type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "vdecl " << sym->name() << " "
      << sym->type()->name();
  }

  void PrintFilter::EnterDeclSymbolVisitor::visit(ptr<Symbol<Type<TypeBase> > >::const_type sym)
  {
    // This is a type that doesn't need a declaration.  So tell the
    // printer not to print a newline after it.
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::EnterDeclSymbolVisitor::visit(ptr<Symbol<Type<StructType> > >::const_type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "tdecl " << sym->name();
    out << " {\n";
    for (Symbol<Type<StructType> >::const_iterator p =
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

  void PrintFilter::EnterDeclSymbolVisitor::visit(ptr<Symbol<Type<Tuple> > >::const_type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "tdecl " << sym->name();
    out << " (\n";
    for (Symbol<Type<StructType> >::const_iterator p =
	   sym->begin(), pend = sym->end();
	 p != pend;
	 /* NULL */) {
      out << indent(ind+IndentFactor) << (*p)->name();
      if (++p != pend) {
        out << ',';
      }
      out << '\n';
    }
    out << indent(ind) << ")";
  }

  /// Print the final newline after each symbol declaration.
  void PrintFilter::LeaveDeclSymbolVisitor::visit(ptr<Symbol<Base> >::const_type)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveDeclSymbolVisitor::visit(ptr<Symbol<Type<TypeBase> > >::const_type)
  {
    // Don't do anything since we didn't print anything for this
    // type.
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  /// Print the final newline after struct types.
  void PrintFilter::LeaveDeclSymbolVisitor::visit(ptr<Symbol<Type<StructType> > >::const_type)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  /// Print the final newline after tuple types.
  void PrintFilter::LeaveDeclSymbolVisitor::visit(ptr<Symbol<Type<Tuple> > >::const_type)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::EnterDefSymbolVisitor::visit(ptr<Symbol<Module> >::const_type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterDefSymbolVisitor::visit(ptr<Symbol<Function> >::const_type sym)
  {
    if (!sym->statementEmpty()) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "fdef " << sym->name() << " {\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }
  }

  void PrintFilter::EnterDefSymbolVisitor::visit(ptr<Symbol<Variable> >::const_type sym)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "vdecl " << sym->name() << " "
      << sym->type()->name();
  }

  void PrintFilter::LeaveDefSymbolVisitor::visit(ptr<Symbol<Variable> >::const_type sym)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
           || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveDefSymbolVisitor::visit(ptr<Symbol<Module> >::const_type sym) {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "}\n";
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveDefSymbolVisitor::visit(ptr<Symbol<Function> >::const_type sym) {
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

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Block> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "{\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<IfThen> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "ifThen\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<IfElse> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "ifElse\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<While> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "while\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<DoWhile> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "doWhile\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Switch> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "switch\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Case> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "case";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<CaseBlock> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "caseblock";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Before> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "before ";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<After> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "after ";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Goto> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "goto ";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Return> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "return\n";
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Assignment> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "assign\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterStatementVisitor::visit(ptr<Statement<Call> >::const_type stmt)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "call\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::LeaveStatementVisitor::visit(ptr<Statement<Base> >::const_type stmt)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveStatementVisitor::visit(ptr<Statement<Block> >::const_type stmt) {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      out << "\n";
    }
    out << indent(ind) << "}\n";
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveStatementVisitor::visit(ptr<Statement<Return> >::const_type stmt) {
    Stream &out = attributeManager.getInheritedAttribute().out();

    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      out << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Add> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "+\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Subtract> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "-\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Multiply> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "*\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Divide> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "/\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Modulus> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "%\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Negate> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "neg\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "&&\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalOr> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "||\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalNot> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "!\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "&\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "|\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "~\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LessThan> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "<\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "<=\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Equal> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "==\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<NotEqual> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "!=\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<GreaterThan> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << ">\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << ">=\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "vref " << expr->getSymbol()->name();
  }

  template<typename ValueType>
  void PrintFilter::EnterExpressionVisitor::visitConstant(boost::shared_ptr<const Expression<Reference<Constant<ValueType> > > > expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "cref "
      << expr->getSymbol()->type()->name()
      << ' ' << expr->getSymbol()->valueString();
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<Base> > > >::const_type expr)
  {
    visitConstant(expr);
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int8_t> > > >::const_type expr)
  {
    visitConstant(expr);
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint8_t> > > >::const_type expr)
  {
    visitConstant(expr);
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int16_t> > > >::const_type expr)
  {
    visitConstant(expr);
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint16_t> > > >::const_type expr)
  {
    visitConstant(expr);
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int32_t> > > >::const_type expr)
  {
    visitConstant(expr);
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint32_t> > > >::const_type expr)
  {
    visitConstant(expr);
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int64_t> > > >::const_type expr)
  {
    visitConstant(expr);
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint64_t> > > >::const_type expr)
  {
    visitConstant(expr);
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<float> > > >::const_type expr)
  {
    visitConstant(expr);
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<double> > > >::const_type expr)
  {
    visitConstant(expr);
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Function> > >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "fref " << expr->getSymbol()->name();
  }

  void PrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Array> > >::const_type expr)
  {
    Stream &out = attributeManager.getInheritedAttribute().out();
    Indent ind = attributeManager.getInheritedAttribute().indent();

    out << indent(ind) << "aref\n";
    attributeManager.setInheritedAttribute(
      InheritedAttribute(ind + IndentFactor, out));
  }

  void PrintFilter::LeaveExpressionVisitor::visit(ptr<Expression<Base> >::const_type expr)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::LeaveExpressionVisitor::visit(ptr<InnerExpression>::const_type expr)
  {
    if (   !attributeManager.setLastSynthesizedAttribute()
        || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
      attributeManager.getInheritedAttribute().out() << "\n";
    }
    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
  }

  void PrintFilter::PrintDefSymbolFlow::visit(ptr<Symbol<Module>>::const_type sym) {
    // We only want to visit functions here since we already
    // declared module-level types and variables.
    this->doEnter(sym);
    // Visit functions
    for(Symbol<Module>::ConstFunctionIterator f = sym->functionBegin(),
          fend = sym->functionEnd();
        f != fend;
        /* NULL */) {
      if (!(*f)->statementEmpty()) {
        this->doBefore(sym, *f);
        (*f)->accept(*this);
        this->doAfter(sym, *f);
      }
      Symbol<Module>::ConstFunctionIterator prev = f;
      if (++f != fend) {
        if (!(*prev)->statementEmpty() && !(*f)->statementEmpty()) {
          this->doBetween(sym, *prev, *f);
        }
      }
    }
    this->doLeave(sym);
  }

  void PrintFilter::operator()(ptr<Node<Base> >::const_type node)
  {
    if (ptr<Symbol<Module> >::const_type s =
        boost::dynamic_pointer_cast<const Symbol<Module> >(node)) {
      ptr<ConstSymbolVisitor>::type declflow(new PrintDeclSymbolFlow(out));
      s->accept(*declflow);
      ptr<ConstSymbolVisitor>::type defflow(new PrintDefSymbolFlow(out));
      s->accept(*defflow);
    }
    else if (ptr<Symbol<Base> >::const_type s =
             boost::dynamic_pointer_cast<const Symbol<Base> >(node)) {
      ptr<ConstSymbolVisitor>::type defflow(new PrintDefSymbolFlow(out));
      s->accept(*defflow);
    }
    else if (ptr<Statement<Base> >::const_type s =
             boost::dynamic_pointer_cast<const Statement<Base> >(node)) {
      ptr<ConstStatementVisitor>::type flow(new PrintFlow(out));
      s->accept(*flow);
    }
    else if (ptr<Expression<Base> >::const_type e =
             boost::dynamic_pointer_cast<const Expression<Base> >(node)) {
      ptr<ConstExpressionVisitor>::type flow(new PrintExpressionFlow(out));
      e->accept(*flow);
    }
  }
}
