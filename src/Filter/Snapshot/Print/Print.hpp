#ifndef mirv_Filter_Snapshot_Print_hpp
#define mirv_Filter_Snapshot_Print_hpp

#include <mirv/Core/Filter/AttributeFlow.hpp>
#include <mirv/Core/Filter/ForwardFlow.hpp>
#include <mirv/Core/Filter/ExpressionFlow.hpp>
#include <mirv/Core/Filter/FlowAction.hpp>
#include <mirv/Core/Filter/SymbolFlow.hpp>
#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/IR/Node.hpp>
#include <mirv/Core/Utility/Debug.hpp>

namespace mirv {
  /// This is a filter to print MIRV IR in textual form.
  class PrintFilter : public Filter<Node<Base> > {
  private:
    typedef std::ostream Stream;
    typedef int Indent;
    /// The factor by which to increase the indent at deeper nesting
    /// levels.
    const static int IndentFactor;     

    /// A handle to the output stream.
    Stream &out;

    /// Define the inherited attibute.
    class InheritedAttribute {
    private:
      /// The current indent level.
      Indent ind;
      /// The stream to dump to.
      Stream *ot;

    public:
      InheritedAttribute(void) : ind(0), ot(0) {}
      InheritedAttribute(Indent i, Stream &o) : ind(i), ot(&o) {}

      Indent indent(void) const {
        return ind;
      }
      Stream &out(void) const {
        checkInvariant(ot != 0, "No output stream");
        return *ot;
      }
    };

    /// Define the synthesized attribute.
    class SynthesizedAttribute {
    private:
      /// This indicates whether we just left an IR subtree.  If so,
      /// we want to suppress printing of a newline.
      bool JustLeft;

    public:
      SynthesizedAttribute(bool value = false) : JustLeft(value) {}

      bool justLeft(void) const {
        return JustLeft;
      }
    };

    typedef FlowAttributeManager<
      InheritedAttribute,
      SynthesizedAttribute
      > FlowAttributeManagerType;

    /// Entering each symbol
    class EnterDeclSymbolVisitor : public SymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterDeclSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Symbol<Module> >::type sym);
      void visit(ptr<Symbol<Function> >::type sym);
      void visit(ptr<Symbol<Variable> >::type sym);
      void visit(ptr<Symbol<Type<TypeBase> > >::type sym);
      void visit(ptr<Symbol<Type<StructType> > >::type sym);
    };

    class EnterDeclSymbolAction : public VisitAction<EnterDeclSymbolVisitor> {
    public:
      EnterDeclSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterDeclSymbolVisitor>(attributeManager) {}
    };

    class EnterDefSymbolVisitor : public SymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterDefSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Symbol<Module> >::type sym);
      void visit(ptr<Symbol<Function> >::type sym);
      void visit(ptr<Symbol<Variable> >::type sym);
    };

    class EnterDefSymbolAction : public VisitAction<EnterDefSymbolVisitor> {
    public:
      EnterDefSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterDefSymbolVisitor>(attributeManager) {}
    };

    /// Leaving each symbol declaration.
    class LeaveDeclSymbolVisitor : public SymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveDeclSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      /// Print the final newline after each symbol declaration.
      void visit(ptr<Symbol<Base> >::type);

      /// Do nothing for most types.
      void visit(ptr<Symbol<Type<TypeBase> >  >::type);

      /// Print the final newline after struct types.
      void visit(ptr<Symbol<Type<StructType> > >::type);
    };

    class LeaveDeclSymbolAction : public VisitAction<LeaveDeclSymbolVisitor> {
    public:
      LeaveDeclSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveDeclSymbolVisitor>(attributeManager) {}
    };

    /// Leaving each symbol definition.
    class LeaveDefSymbolVisitor : public SymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveDefSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      /// Print the final newline after each symbol definition.
      void visit(ptr<Symbol<Variable> >::type sym);
      void visit(ptr<Symbol<Module> >::type sym);
      void visit(ptr<Symbol<Function> >::type sym);
    };

    class LeaveDefSymbolAction : public VisitAction<LeaveDefSymbolVisitor> {
    public:
      LeaveDefSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveDefSymbolVisitor>(attributeManager) {}
    };

    /// Entering each statement
    class EnterStatementVisitor : public StatementVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterStatementVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Statement<Block> >::type stmt);
      void visit(ptr<Statement<IfThen> >::type stmt);
      void visit(ptr<Statement<IfElse> >::type stmt);
      void visit(ptr<Statement<While> >::type stmt);
      void visit(ptr<Statement<DoWhile> >::type stmt);
      void visit(ptr<Statement<Switch> >::type stmt);
      void visit(ptr<Statement<Case> >::type stmt);
      void visit(ptr<Statement<CaseBlock> >::type stmt);
      void visit(ptr<Statement<Before> >::type stmt);
      void visit(ptr<Statement<After> >::type stmt);
      void visit(ptr<Statement<Goto> >::type stmt);
      void visit(ptr<Statement<Return> >::type stmt);
      void visit(ptr<Statement<Assignment> >::type stmt);
    };

    class EnterStatementAction : public VisitAction<EnterStatementVisitor> {
    public:
      EnterStatementAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterStatementVisitor>(attributeManager) {}
    };

    /// Leaving each statement
    class LeaveStatementVisitor : public StatementVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveStatementVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Statement<Base> >::type stmt);
      void visit(ptr<Statement<Block> >::type stmt);
      void visit(ptr<Statement<Return> >::type stmt);
    };

    class LeaveStatementAction : public VisitAction<LeaveStatementVisitor> {
    public:
      LeaveStatementAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveStatementVisitor>(attributeManager) {}
    };

    /// Entering each expression
    class EnterExpressionVisitor : public ExpressionVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterExpressionVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Expression<Add> >::type expr);
      void visit(ptr<Expression<Subtract> >::type expr);
      void visit(ptr<Expression<Multiply> >::type expr);
      void visit(ptr<Expression<Divide> >::type expr);
      void visit(ptr<Expression<Modulus> >::type expr);
      void visit(ptr<Expression<Negate> >::type expr);
      void visit(ptr<Expression<LogicalAnd> >::type expr);
      void visit(ptr<Expression<LogicalOr> >::type expr);
      void visit(ptr<Expression<LogicalNot> >::type expr);
      void visit(ptr<Expression<BitwiseAnd> >::type expr);
      void visit(ptr<Expression<BitwiseOr> >::type expr);
      void visit(ptr<Expression<BitwiseComplement> >::type expr);
      void visit(ptr<Expression<LessThan> >::type expr);
      void visit(ptr<Expression<LessThanOrEqual> >::type expr);
      void visit(ptr<Expression<Equal> >::type expr);
      void visit(ptr<Expression<NotEqual> >::type expr);
      void visit(ptr<Expression<GreaterThan> >::type expr);
      void visit(ptr<Expression<GreaterThanOrEqual> >::type expr);
      void visit(ptr<Expression<Reference<Variable> > >::type expr);
    };

    class EnterExpressionAction : public VisitAction<EnterExpressionVisitor> {
    public:
      EnterExpressionAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterExpressionVisitor>(attributeManager) {}
    };

    /// Leaving each expression
    class LeaveExpressionVisitor : public ExpressionVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveExpressionVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Expression<Base> >::type expr);
      void visit(ptr<InnerExpression>::type expr);
    };

    class LeaveExpressionAction : public VisitAction<LeaveExpressionVisitor> {
    public:
      LeaveExpressionAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveExpressionVisitor>(attributeManager) {}
    };

    /// This is the flow to print expressions.
    class PrintExpressionFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardExpressionFlowGenerator,
      EnterExpressionAction,
      LeaveExpressionAction,
      NullAction,
      NullAction,
      NullAction
      > {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardExpressionFlowGenerator,
      EnterExpressionAction,
      LeaveExpressionAction,
      NullAction,
      NullAction,
      NullAction
      > BaseType;

    public:
      PrintExpressionFlow(Stream &out)
          : BaseType(InheritedAttribute(0, out)) {}

      PrintExpressionFlow(FlowAttributeManagerType &)
          : BaseType(InheritedAttribute()) {}        

    };

    /// This is the flow to print statements.
    class PrintFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardFlowGenerator,
      EnterStatementAction,
      LeaveStatementAction,
      NullAction,
      NullAction,
      NullAction,
      NullJoinAction,
      NullAction,
      FlowAction<PrintFlow, PrintExpressionFlow>,
      NullAction
      >,
         public boost::enable_shared_from_this<PrintFlow> {
      typedef AttributeFlow<
        InheritedAttribute,
        SynthesizedAttribute,
        ForwardFlowGenerator,
        EnterStatementAction,
        LeaveStatementAction,
        NullAction,
        NullAction,
        NullAction,
        NullJoinAction,
        NullAction,
        FlowAction<PrintFlow, PrintExpressionFlow>,
        NullAction
        > BaseType;

    public:
      PrintFlow(Stream &out) : BaseType(InheritedAttribute(0, out)) {
        expression().setParentFlow(this);
      }

      PrintFlow(FlowAttributeManagerType &)
          : BaseType(InheritedAttribute()) {
        expression().setParentFlow(this);
      }

      // We need to reverse the order in which we visit the
      // assignment operands.
      void visit(ptr<Statement<Assignment> >::type stmt) {
        this->doEnter(stmt);

        this->doBeforeExpression(stmt, stmt->getLeftExpression());
        this->doExpression(stmt, stmt->getLeftExpression());
        this->doAfterExpression(stmt, stmt->getLeftExpression());

        this->doBeforeExpression(stmt, stmt->getRightExpression());
        this->doExpression(stmt, stmt->getRightExpression());
        this->doAfterExpression(stmt, stmt->getRightExpression());

        this->doLeave(stmt);
      } 
    };

    /// This is the flow to print symbol declarations.  It prints the
    /// module definition since there is no such thing as a module
    /// declaration.
    class PrintDeclSymbolFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      SymbolFlowGenerator,
      EnterDeclSymbolAction,
      LeaveDeclSymbolAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction> {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      SymbolFlowGenerator,
      EnterDeclSymbolAction,
      LeaveDeclSymbolAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction> BaseType;

    public:
      PrintDeclSymbolFlow(Stream &out)
          : BaseType(InheritedAttribute(0, out)) {}

      // We just want the function declaration at this point so don't
      // visit children.
      void visit(ptr<Symbol<Function> >::type sym) {
        this->doEnter(sym);
        this->doLeave(sym);
      }
    };

    /// This is the flow to print symbol defintions.
    class PrintDefSymbolFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      SymbolFlowGenerator,
      EnterDefSymbolAction,
      LeaveDefSymbolAction,
      NullAction,
      NullAction,
      NullAction,
      FlowAction<PrintDefSymbolFlow, PrintFlow>
      >,
         public boost::enable_shared_from_this<PrintDefSymbolFlow> {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      SymbolFlowGenerator,
      EnterDefSymbolAction,
      LeaveDefSymbolAction,
      NullAction,
      NullAction,
      NullAction,
      FlowAction<PrintDefSymbolFlow, PrintFlow>
      > BaseType;

    public:
      PrintDefSymbolFlow(Stream &out, Indent i = 0)
          : BaseType(InheritedAttribute(i, out)) {
        statement().setParentFlow(this);
      }

      /// We only want to visit functions here since we already
      /// declared module-level types and variables.
      void visit(ptr<Symbol<Module> >::type sym);
    };

  public:
    PrintFilter(Stream &o) : out(o) {}

    /// This is an iostream manipulator to print a specified number
    /// of spaces for indentation.
    class indent {
    private:
      /// The number of spaces to print.
      int val;

    public:
      indent(int ind) : val(ind) {};

      /// Print the spafces specified by val.
      Stream &operator()(Stream &out) const {
        checkInvariant(val >= 0, "Indent underflow");
        int i = val;
        while(i--) {
          out << " ";
        }
        return(out);
      }
    };

    void operator()(ptr<Node<Base> >::type node);
  };

  /// This is the stream operator for the indent iostream manipulator.
  inline std::ostream &operator<<(std::ostream &out, const PrintFilter::indent &ind) {
    return ind(out);
  }
}

#endif
