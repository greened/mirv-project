#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Filter/AttributeFlow.hpp>
#include <mirv/Core/Filter/ConstForwardStatementFlow.hpp>
#include <mirv/Core/Filter/ConstExpressionFlow.hpp>
#include <mirv/Core/Filter/FlowAction.hpp>
#include <mirv/Core/Filter/ConstSymbolFlow.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Utility/Printer.hpp>
#include <mirv/Core/Utility/Debug.hpp>

namespace mirv {
  namespace Printer {
    typedef std::vector<ptr<Symbol<Type<TypeBase> > >::const_type> TypeList;
    typedef int Indent;

    /// Define the inherited attibute.
    class TypeNameInheritedAttribute {
    private:
      /// The stream to dump to.
      Stream *ot;
      TypeList *ts;

      TypeList &TypeStack(void) const {
        checkInvariant(ts != 0, "No type stack");
        return *ts;
      }

    public:
      TypeNameInheritedAttribute(void) : ot(0), ts(0) {}
      TypeNameInheritedAttribute(Stream &o, TypeList &t)
          : ot(&o), ts(&t) {}

      Stream &out(void) const {
        checkInvariant(ot != 0, "No output stream");
        return *ot;
      }

      void typeStackPushBack(ptr<Symbol<Type<TypeBase> > >::const_type type) const {
        TypeStack().push_back(type);
      }
      void typeStackPopBack(void) const {
        TypeStack().pop_back();
      }

      typedef TypeList::const_reverse_iterator TypeStackIterator;

      TypeStackIterator typeStackBegin(void) const {
        return TypeStack().rbegin();
      }

      TypeStackIterator typeStackEnd(void) const {
        return TypeStack().rend();
      }

      TypeStackIterator
      typeStackFind(ptr<Symbol<Type<TypeBase> > >::const_type type) const {
        return std::find(typeStackBegin(), typeStackEnd(), type);
      }
    };

    /// Define the synthesized attribute.
    class TypeNameSynthesizedAttribute {
    private:
      TypeList *ts;

    public:
      TypeNameSynthesizedAttribute(void) : ts(0) {}
      TypeNameSynthesizedAttribute(TypeList &t) : ts(&t) {}
    };

    typedef FlowAttributeManager<
      TypeNameInheritedAttribute,
      TypeNameSynthesizedAttribute
      > TypeNameFlowAttributeManagerType;

    /// Type name construction.
    class EnterTypeNameVisitor : public ConstSymbolVisitor {
    private:
      TypeNameFlowAttributeManagerType &attributeManager;

    public:
      EnterTypeNameVisitor(TypeNameFlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Symbol<Type<Integral> > >::const_type);
      void visit(ptr<Symbol<Type<Floating> > >::const_type);
      void visit(ptr<Symbol<Type<Tuple> > >::const_type);
      void visit(ptr<Symbol<Type<Derived> > >::const_type);
    };

    class EnterTypeNameAction : public VisitAction<EnterTypeNameVisitor> {
    public:
      EnterTypeNameAction(TypeNameFlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterTypeNameVisitor>(attributeManager) {}
    };

    class BeforeTypeNameVisitor : public ConstSymbolVisitor {
    private:
      TypeNameFlowAttributeManagerType &attributeManager;

    public:
      BeforeTypeNameVisitor(TypeNameFlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Symbol<Type<TypeBase> > >::const_type);
      void visit(ptr<Symbol<Type<Derived> > >::const_type);
    };

    class BeforeTypeNameAction : public VisitAction<BeforeTypeNameVisitor> {
    public:
      BeforeTypeNameAction(TypeNameFlowAttributeManagerType &attributeManager) 
          : VisitAction<BeforeTypeNameVisitor>(attributeManager) {}
    };

    class AfterTypeNameAction {
    private:
      TypeNameFlowAttributeManagerType &attributeManager;

    public:
      AfterTypeNameAction(TypeNameFlowAttributeManagerType &am) 
          : attributeManager(am) {}

      typedef void result_type;

      template<typename Parent, typename InputIterator>
      void operator()(boost::shared_ptr<Parent> parent,
                      InputIterator node) {}

      template<typename InputIterator>
      void operator()(ptr<InnerType>::const_type parent,
                      InputIterator node) {
        Stream &out = attributeManager.getInheritedAttribute().out();
        if (ptr<Symbol<Type<FunctionType> > >::const_type function =
            dyn_cast<const Symbol<Type<FunctionType> > >(parent)) {
          if (node == function->begin()) {
            if (!*node) {
              out << "void (";
            }
            else {
              out << " (";
            }
          }
        }
      } 
    };

    class BetweenTypeNameAction {
    private:
      TypeNameFlowAttributeManagerType &attributeManager;

    public:
      BetweenTypeNameAction(TypeNameFlowAttributeManagerType &am) 
          : attributeManager(am) {}

      typedef void result_type;

      template<typename InputIterator>
      void operator()(boost::shared_ptr<const Symbol<Module> > parent,
                      InputIterator node,
                      InputIterator next) {}

      template<typename InputIterator>
      void operator()(boost::shared_ptr<const Symbol<Function> > parent,
                      InputIterator node,
                      InputIterator next) {}

      template<typename InputIterator>
      void operator()(boost::shared_ptr<const InnerSymbol> parent,
                      InputIterator node,
                      InputIterator next) {}

      template<typename InputIterator>
      void operator()(boost::shared_ptr<const InnerType> parent,
                      InputIterator node,
                      InputIterator next) {
        Stream &out = attributeManager.getInheritedAttribute().out();
        if (ptr<Symbol<Type<FunctionType> > >::const_type function =
            dyn_cast<const Symbol<Type<FunctionType> > >(parent)) {
          if (node == function->begin()) {
            return;
          }
        }
        out << ", ";
      }
    };

    class LeaveTypeNameVisitor : public ConstSymbolVisitor {
    private:
      TypeNameFlowAttributeManagerType &attributeManager;

    public:
      LeaveTypeNameVisitor(TypeNameFlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Symbol<Type<Pointer> > >::const_type);
      void visit(ptr<Symbol<Type<FunctionType> > >::const_type);
      void visit(ptr<Symbol<Type<Tuple> > >::const_type);
      void visit(ptr<Symbol<Type<Derived> > >::const_type);
    };

    class LeaveTypeNameAction : public VisitAction<LeaveTypeNameVisitor> {
    public:
      LeaveTypeNameAction(TypeNameFlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveTypeNameVisitor>(attributeManager) {}
    };

    class TypeNameFlow : public AttributeFlow<
      TypeNameInheritedAttribute,
      TypeNameSynthesizedAttribute,
      ConstSymbolFlowGenerator,
      EnterTypeNameAction,
      LeaveTypeNameAction,
      BeforeTypeNameAction,
      AfterTypeNameAction,
      BetweenTypeNameAction,
      NullAction> {
    private:
      typedef AttributeFlow<
      TypeNameInheritedAttribute,
      TypeNameSynthesizedAttribute,
      ConstSymbolFlowGenerator,
      EnterTypeNameAction,
      LeaveTypeNameAction,
      BeforeTypeNameAction,
      AfterTypeNameAction,
      BetweenTypeNameAction,
      NullAction> BaseType;

      Printer::TypeList typeStack;

    public:
      TypeNameFlow(std::ostream &out)
          : BaseType(TypeNameInheritedAttribute(out, typeStack)) {}
    };

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
    class EnterDeclSymbolVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterDeclSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Symbol<Module> >::const_type sym);
      void visit(ptr<Symbol<Function> >::const_type sym);
      void visit(ptr<Symbol<Variable> >::const_type sym);
      void visit(ptr<Symbol<Type<TypeBase> > >::const_type sym);
      void visit(ptr<Symbol<Type<Tuple> > >::const_type sym);
    };

    class EnterDeclSymbolAction : public VisitAction<EnterDeclSymbolVisitor> {
    public:
      EnterDeclSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterDeclSymbolVisitor>(attributeManager) {}
    };

    class EnterDefSymbolVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterDefSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Symbol<Module> >::const_type sym);
      void visit(ptr<Symbol<Function> >::const_type sym);
      void visit(ptr<Symbol<Variable> >::const_type sym);
    };

    class EnterDefSymbolAction : public VisitAction<EnterDefSymbolVisitor> {
    public:
      EnterDefSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterDefSymbolVisitor>(attributeManager) {}
    };

    /// Leaving each symbol declaration.
    class LeaveDeclSymbolVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveDeclSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      /// Print the final newline after each symbol declaration.
      void visit(ptr<Symbol<Base> >::const_type);

      /// Do nothing for most types.
      void visit(ptr<Symbol<Type<TypeBase> >  >::const_type);

      /// Print the final newline after tuple types.
      void visit(ptr<Symbol<Type<Tuple> > >::const_type);
    };

    class LeaveDeclSymbolAction : public VisitAction<LeaveDeclSymbolVisitor> {
    public:
      LeaveDeclSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveDeclSymbolVisitor>(attributeManager) {}
    };

    /// Leaving each symbol definition.
    class LeaveDefSymbolVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveDefSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      /// Print the final newline after each symbol definition.
      void visit(ptr<Symbol<Variable> >::const_type sym);
      void visit(ptr<Symbol<Module> >::const_type sym);
      void visit(ptr<Symbol<Function> >::const_type sym);
    };

    class LeaveDefSymbolAction : public VisitAction<LeaveDefSymbolVisitor> {
    public:
      LeaveDefSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveDefSymbolVisitor>(attributeManager) {}
    };

    /// Entering each statement
    class EnterStatementVisitor : public ConstStatementVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterStatementVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Statement<Block> >::const_type stmt);
      void visit(ptr<Statement<IfThen> >::const_type stmt);
      void visit(ptr<Statement<IfElse> >::const_type stmt);
      void visit(ptr<Statement<While> >::const_type stmt);
      void visit(ptr<Statement<DoWhile> >::const_type stmt);
      void visit(ptr<Statement<Switch> >::const_type stmt);
      void visit(ptr<Statement<Case> >::const_type stmt);
      void visit(ptr<Statement<CaseBlock> >::const_type stmt);
      void visit(ptr<Statement<Before> >::const_type stmt);
      void visit(ptr<Statement<After> >::const_type stmt);
      void visit(ptr<Statement<Goto> >::const_type stmt);
      void visit(ptr<Statement<Return> >::const_type stmt);
      void visit(ptr<Statement<Assignment> >::const_type stmt);
      void visit(ptr<Statement<Call> >::const_type stmt);
    };

    class EnterStatementAction : public VisitAction<EnterStatementVisitor> {
    public:
      EnterStatementAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterStatementVisitor>(attributeManager) {}
    };

    /// Leaving each statement
    class LeaveStatementVisitor : public ConstStatementVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveStatementVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Statement<Base> >::const_type stmt);
      void visit(ptr<Statement<Block> >::const_type stmt);
      void visit(ptr<Statement<Return> >::const_type stmt);
    };

    class LeaveStatementAction : public VisitAction<LeaveStatementVisitor> {
    public:
      LeaveStatementAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveStatementVisitor>(attributeManager) {}
    };

    /// Entering each expression
    class EnterExpressionVisitor : public ConstExpressionVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

      template<typename ValueType>
      void visitConstant(boost::shared_ptr<const Expression<Reference<Constant<ValueType> > > > expr);

    public:
      EnterExpressionVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Expression<Add> >::const_type expr);
      void visit(ptr<Expression<Subtract> >::const_type expr);
      void visit(ptr<Expression<Multiply> >::const_type expr);
      void visit(ptr<Expression<Divide> >::const_type expr);
      void visit(ptr<Expression<Modulus> >::const_type expr);
      void visit(ptr<Expression<Negate> >::const_type expr);
      void visit(ptr<Expression<LogicalAnd> >::const_type expr);
      void visit(ptr<Expression<LogicalOr> >::const_type expr);
      void visit(ptr<Expression<LogicalNot> >::const_type expr);
      void visit(ptr<Expression<BitwiseAnd> >::const_type expr);
      void visit(ptr<Expression<BitwiseOr> >::const_type expr);
      void visit(ptr<Expression<BitwiseComplement> >::const_type expr);
      void visit(ptr<Expression<LessThan> >::const_type expr);
      void visit(ptr<Expression<LessThanOrEqual> >::const_type expr);
      void visit(ptr<Expression<Equal> >::const_type expr);
      void visit(ptr<Expression<NotEqual> >::const_type expr);
      void visit(ptr<Expression<GreaterThan> >::const_type expr);
      void visit(ptr<Expression<GreaterThanOrEqual> >::const_type expr);
      void visit(ptr<Expression<TuplePointer> >::const_type expr);
      void visit(ptr<Expression<Reference<Variable> > >::const_type expr);
      void visit(ptr<Expression<Reference<Function> > >::const_type expr);
      void visit(ptr<Expression<Reference<Tuple> > >::const_type expr);
      void visit(ptr<Expression<Reference<Constant<std::int8_t> > > >::const_type expr);
      void visit(ptr<Expression<Reference<Constant<std::uint8_t> > > >::const_type expr);
      void visit(ptr<Expression<Reference<Constant<std::int16_t> > > >::const_type expr);
      void visit(ptr<Expression<Reference<Constant<std::uint16_t> > > >::const_type expr);
      void visit(ptr<Expression<Reference<Constant<std::int32_t> > > >::const_type expr);
      void visit(ptr<Expression<Reference<Constant<std::uint32_t> > > >::const_type expr);
      void visit(ptr<Expression<Reference<Constant<std::int64_t> > > >::const_type expr);
      void visit(ptr<Expression<Reference<Constant<std::uint64_t> > > >::const_type expr);
      void visit(ptr<Expression<Reference<Constant<float> > > >::const_type expr);
      void visit(ptr<Expression<Reference<Constant<double> > > >::const_type expr);
      void visit(ptr<Expression<Reference<Constant<Base> > > >::const_type expr);
    };

    class EnterExpressionAction : public VisitAction<EnterExpressionVisitor> {
    public:
      EnterExpressionAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterExpressionVisitor>(attributeManager) {}
    };

    /// Leaving each expression
    class LeaveExpressionVisitor : public ConstExpressionVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveExpressionVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Expression<Base> >::const_type expr);
      void visit(ptr<InnerExpression>::const_type expr);
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
      ConstForwardExpressionFlowGenerator,
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
      ConstForwardExpressionFlowGenerator,
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
      ConstForwardStatementFlowGenerator,
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
        ConstForwardStatementFlowGenerator,
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
      void visit(ptr<Statement<Assignment> >::const_type stmt) {
        this->doEnter(stmt);

        for (auto i = stmt->expressionBegin();
             i != stmt->expressionEnd(); ++i) {
          this->doBeforeExpression(stmt, i);
          this->doExpression(stmt, i);
          this->doAfterExpression(stmt, i);
        }

        this->doLeave(stmt);
      }
    };

    /// This is the flow to print symbol declarations.  It prints the
    /// module definition since there is no such thing as a module
    /// declaration.
    class PrintDeclSymbolFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstSymbolFlowGenerator,
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
      ConstSymbolFlowGenerator,
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
      void visit(ptr<Symbol<Function> >::const_type sym) {
        this->doEnter(sym);
        this->doLeave(sym);
      }
    };

    /// This is the flow to print symbol defintions.
    class PrintDefSymbolFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstSymbolFlowGenerator,
      EnterDefSymbolAction,
      LeaveDefSymbolAction,
      NullAction,
      NullAction,
      NullAction,
      FlowAction<PrintDefSymbolFlow, PrintFlow>
      >, public boost::enable_shared_from_this<PrintDefSymbolFlow> {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstSymbolFlowGenerator,
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
      void visit(ptr<Symbol<Module> >::const_type sym);
    };

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

    /// This is the stream operator for the indent iostream manipulator.
    inline std::ostream &operator<<(std::ostream &out, const indent &ind) {
      return ind(out);
    }

    /// The factor by which to increase the indent at deeper nesting
    /// levels.
    const int IndentFactor = 3;

    namespace detail {
      namespace {
        /// This is essentially a specialized print filter for
        /// expressions.
        class ExpressionPrintFilter : public ConstFilter<Node<Base> > {
        private:
          typedef std::ostream Stream;
          /// A handle to the output stream.
          Stream &out;

          /// Define the inherited attibute.
          class InheritedAttribute {
          private:
            /// The stream to dump to.
            Stream *ot;

          public:
            InheritedAttribute(void) : ot(0) {}
            InheritedAttribute(Stream &o) : ot(&o) {}

            Stream &out(void) const {
              checkInvariant(ot != 0, "No output stream");
              return *ot;
            }
          };

          struct SynthesizedAttribute {};

          typedef FlowAttributeManager<
            InheritedAttribute,
            SynthesizedAttribute
            > FlowAttributeManagerType;

          /// Entering each expression
          class EnterExpressionVisitor : public ConstExpressionVisitor {
          private:
            FlowAttributeManagerType &attributeManager;
            bool printed;

            template<typename ValueType>
            void visitConstant(boost::shared_ptr<const Expression<Reference<Constant<ValueType> > > > expr);

          public:
            EnterExpressionVisitor(FlowAttributeManagerType &am)
                : attributeManager(am), printed(false) {}

            void visit(ptr<Expression<Add> >::const_type expr);
            void visit(ptr<Expression<Subtract> >::const_type expr);
            void visit(ptr<Expression<Multiply> >::const_type expr);
            void visit(ptr<Expression<Divide> >::const_type expr);
            void visit(ptr<Expression<Modulus> >::const_type expr);
            void visit(ptr<Expression<Negate> >::const_type expr);
            void visit(ptr<Expression<LogicalAnd> >::const_type expr);
            void visit(ptr<Expression<LogicalOr> >::const_type expr);
            void visit(ptr<Expression<LogicalNot> >::const_type expr);
            void visit(ptr<Expression<BitwiseAnd> >::const_type expr);
            void visit(ptr<Expression<BitwiseOr> >::const_type expr);
            void visit(ptr<Expression<BitwiseComplement> >::const_type expr);
            void visit(ptr<Expression<LessThan> >::const_type expr);
            void visit(ptr<Expression<LessThanOrEqual> >::const_type expr);
            void visit(ptr<Expression<Equal> >::const_type expr);
            void visit(ptr<Expression<NotEqual> >::const_type expr);
            void visit(ptr<Expression<GreaterThan> >::const_type expr);
            void visit(ptr<Expression<GreaterThanOrEqual> >::const_type expr);
            void visit(ptr<Expression<TuplePointer> >::const_type expr);
            void visit(ptr<Expression<Reference<Variable> > >::const_type expr);
            void visit(ptr<Expression<Reference<Function> > >::const_type expr);
            void visit(ptr<Expression<Reference<Tuple> > >::const_type expr);
            void visit(ptr<Expression<Reference<Constant<std::int8_t> > > >::const_type expr);
            void visit(ptr<Expression<Reference<Constant<std::uint8_t> > > >::const_type expr);
            void visit(ptr<Expression<Reference<Constant<std::int16_t> > > >::const_type expr);
            void visit(ptr<Expression<Reference<Constant<std::uint16_t> > > >::const_type expr);
            void visit(ptr<Expression<Reference<Constant<std::int32_t> > > >::const_type expr);
            void visit(ptr<Expression<Reference<Constant<std::uint32_t> > > >::const_type expr);
            void visit(ptr<Expression<Reference<Constant<std::int64_t> > > >::const_type expr);
            void visit(ptr<Expression<Reference<Constant<std::uint64_t> > > >::const_type expr);
            void visit(ptr<Expression<Reference<Constant<float> > > >::const_type expr);
            void visit(ptr<Expression<Reference<Constant<double> > > >::const_type expr);
            void visit(ptr<Expression<Reference<Constant<Base> > > >::const_type expr);
          };

          class EnterExpressionAction : public VisitAction<EnterExpressionVisitor> {
          public:
            EnterExpressionAction(FlowAttributeManagerType &attributeManager) 
                : VisitAction<EnterExpressionVisitor>(attributeManager) {}
          };

          /// This is the flow to print expressions.
          class PrintExpressionFlow : public AttributeFlow<
            InheritedAttribute,
            SynthesizedAttribute,
            ConstForwardExpressionFlowGenerator,
            EnterExpressionAction,
            NullAction,
            NullAction,
            NullAction,
            NullAction
            > {
          private:
            typedef AttributeFlow<
            InheritedAttribute,
            SynthesizedAttribute,
            ConstForwardExpressionFlowGenerator,
            EnterExpressionAction,
            NullAction,
            NullAction,
            NullAction,
            NullAction
            > BaseType;
          
          public:
            PrintExpressionFlow(Stream &out)
                : BaseType(InheritedAttribute(out)) {}

            PrintExpressionFlow(FlowAttributeManagerType &)
                : BaseType(InheritedAttribute()) {}        

          };

        public:
          ExpressionPrintFilter(Stream &o) : out(o) {}

          void operator()(ptr<Node<Base> >::const_type node) {
            ptr<Expression<Base> >::const_type e =
              safe_cast<const Expression<Base> >(node);
            ptr<ConstExpressionVisitor>::type flow(new PrintExpressionFlow(out));
            e->accept(*flow);
          }
        };

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Add> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " +" : "+");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Subtract> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " -" : "-");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Multiply> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " *" : "*");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Divide> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " /" : "/");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Modulus> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " %" : "%");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Negate> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " neg" : "neg");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " &&" : "&&");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalOr> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " ||" : "||");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LogicalNot> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " !" : "!");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " &" : "&");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " |" : "|");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " ~" : "~");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LessThan> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " <" : "<");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " <=" : "<=");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Equal> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " ==" : "=="); 
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<NotEqual> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " !=" : "!=");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<GreaterThan> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " >" : ">");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " >=" : ">=");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<TuplePointer> >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " &" : "&");
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " " : "") << expr->getSymbol()->name();
          printed = true;
        }

        template<typename ValueType>
        void ExpressionPrintFilter::EnterExpressionVisitor::visitConstant(boost::shared_ptr<const Expression<Reference<Constant<ValueType> > > > expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " " : "") << expr->getSymbol()->valueString();
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<Base> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int8_t> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint8_t> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int16_t> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint16_t> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int32_t> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint32_t> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int64_t> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint64_t> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<float> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<double> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::string> > > >::const_type expr)
        {
          visitConstant(expr);
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Function> > >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " " : "") << expr->getSymbol()->name();
          printed = true;
        }

        void ExpressionPrintFilter::EnterExpressionVisitor::visit(ptr<Expression<Reference<Tuple> > >::const_type expr)
        {
          Stream &out = attributeManager.getInheritedAttribute().out();
          out << (printed ? " tref" : "tref");
          printed = true;
        }
      }

      std::string stringize(ptr<Expression<Base> >::type expr)
      {
        std::stringstream stream;
        ExpressionPrintFilter print(stream);
        print(expr);
        return stream.str();
      }
    }

    void EnterTypeNameVisitor::visit(ptr<Symbol<Type<Integral> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << "int" << sym->integerBitSize();
    }

    void EnterTypeNameVisitor::visit(ptr<Symbol<Type<Floating> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << "float" << sym->integerBitSize();
    }

    void EnterTypeNameVisitor::visit(ptr<Symbol<Type<Tuple> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << "(";
      if (sym->isUniform()) {
        detail::ExpressionPrintFilter expressionPrinter(out);
        expressionPrinter(sym->count());
        out << " x ";
      }
      attributeManager.getInheritedAttribute().typeStackPushBack(sym);
    }

    void EnterTypeNameVisitor::visit(ptr<Symbol<Type<Derived> > >::const_type sym)
    {
      attributeManager.getInheritedAttribute().typeStackPushBack(sym);
    }

    void BeforeTypeNameVisitor::visit(ptr<Symbol<Type<TypeBase> > >::const_type sym)
    {
      if (!sym) {
        // This is an empty return type.
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "void";
      }
    }

    void BeforeTypeNameVisitor::visit(ptr<Symbol<Type<Derived> > >::const_type sym)
    {
      TypeNameInheritedAttribute::TypeStackIterator i =
        attributeManager.getInheritedAttribute().typeStackFind(sym);
    
      if (i != attributeManager.getInheritedAttribute().typeStackEnd()) {
        Stream &out = attributeManager.getInheritedAttribute().out();

        int level =
          std::distance(attributeManager.getInheritedAttribute().typeStackBegin(),
                        i) + 1;
        out << '^' << level;
      }
    }

    void LeaveTypeNameVisitor::visit(ptr<Symbol<Type<Pointer> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << " *";
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    void LeaveTypeNameVisitor::visit(ptr<Symbol<Type<FunctionType> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      if (sym->isVararg()) {
        out << ", ...";
      }
      out << ")";
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    void LeaveTypeNameVisitor::visit(ptr<Symbol<Type<Tuple> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << ")";
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    void LeaveTypeNameVisitor::visit(ptr<Symbol<Type<Derived> > >::const_type sym)
    {
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    void EnterDeclSymbolVisitor::visit(ptr<Symbol<Module> >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "mdef " << sym->name() << " {\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterDeclSymbolVisitor::visit(ptr<Symbol<Function> >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      // Just declarations.
      out << indent(ind) << "fdecl " << sym->name() << ' ';
      TypeNameFlow typePrinter(out);
      sym->type()->accept(typePrinter);
    }

    void EnterDeclSymbolVisitor::visit(ptr<Symbol<Variable> >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "vdecl " << sym->name() << " "; 
      TypeNameFlow typePrinter(out);
      sym->type()->accept(typePrinter);
    }

    void EnterDeclSymbolVisitor::visit(ptr<Symbol<Type<TypeBase> > >::const_type sym)
    {
      // This is a type that doesn't need a declaration.  So tell the
      // printer not to print a newline after it.
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    void EnterDeclSymbolVisitor::visit(ptr<Symbol<Type<Tuple> > >::const_type sym)
    {
      // Stream &out = attributeManager.getInheritedAttribute().out();
      // Indent ind = attributeManager.getInheritedAttribute().indent();

      // out << indent(ind) << "tdecl " << sym->name();
      // out << " (\n";
      // for (Symbol<Type<Tuple> >::const_iterator p =
      //        sym->begin(), pend = sym->end();
      //      p != pend;
      //      /* NULL */) {
      //   out << indent(ind+IndentFactor) << (*p)->name();
      //   if (++p != pend) {
      //     out << ',';
      //   }
      //   out << '\n';
      // }
      // out << indent(ind) << ")";
    }

    /// Print the final newline after each symbol declaration.
    void LeaveDeclSymbolVisitor::visit(ptr<Symbol<Base> >::const_type)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out() << "\n";
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    void LeaveDeclSymbolVisitor::visit(ptr<Symbol<Type<TypeBase> > >::const_type)
    {
      // Don't do anything since we didn't print anything for this
      // type.
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    /// Print the final newline after tuple types.
    void LeaveDeclSymbolVisitor::visit(ptr<Symbol<Type<Tuple> > >::const_type)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out() << "\n";
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    void EnterDefSymbolVisitor::visit(ptr<Symbol<Module> >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterDefSymbolVisitor::visit(ptr<Symbol<Function> >::const_type sym)
    {
      if (!sym->statementEmpty()) {
        Stream &out = attributeManager.getInheritedAttribute().out();
        Indent ind = attributeManager.getInheritedAttribute().indent();

        out << indent(ind) << "fdef " << sym->name() << " {\n";
        attributeManager.setInheritedAttribute(
          InheritedAttribute(ind + IndentFactor, out));
      }
    }

    void EnterDefSymbolVisitor::visit(ptr<Symbol<Variable> >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "vdecl " << sym->name() << " ";
      TypeNameFlow typePrinter(out);
      sym->type()->accept(typePrinter);
    }

    void LeaveDefSymbolVisitor::visit(ptr<Symbol<Variable> >::const_type sym)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out() << "\n";
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    void LeaveDefSymbolVisitor::visit(ptr<Symbol<Module> >::const_type sym) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "}\n";
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    void LeaveDefSymbolVisitor::visit(ptr<Symbol<Function> >::const_type sym) {
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

    void EnterStatementVisitor::visit(ptr<Statement<Block> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "{\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterStatementVisitor::visit(ptr<Statement<IfThen> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "ifThen\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterStatementVisitor::visit(ptr<Statement<IfElse> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "ifElse\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterStatementVisitor::visit(ptr<Statement<While> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "while\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterStatementVisitor::visit(ptr<Statement<DoWhile> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "doWhile\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterStatementVisitor::visit(ptr<Statement<Switch> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "switch\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterStatementVisitor::visit(ptr<Statement<Case> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "case";
    }

    void EnterStatementVisitor::visit(ptr<Statement<CaseBlock> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "caseblock";
    }

    void EnterStatementVisitor::visit(ptr<Statement<Before> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "before ";
    }

    void EnterStatementVisitor::visit(ptr<Statement<After> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "after ";
    }

    void EnterStatementVisitor::visit(ptr<Statement<Goto> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "goto ";
    }

    void EnterStatementVisitor::visit(ptr<Statement<Return> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "return\n";
    }

    void EnterStatementVisitor::visit(ptr<Statement<Assignment> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "assign\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterStatementVisitor::visit(ptr<Statement<Call> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "call\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void LeaveStatementVisitor::visit(ptr<Statement<Base> >::const_type stmt)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out() << "\n";
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    void LeaveStatementVisitor::visit(ptr<Statement<Block> >::const_type stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << "\n";
      }
      out << indent(ind) << "}\n";
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    void LeaveStatementVisitor::visit(ptr<Statement<Return> >::const_type stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();

      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << "\n";
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Add> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "+\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Subtract> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "-\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Multiply> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "*\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Divide> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "/\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Modulus> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "%\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Negate> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "neg\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "&&\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<LogicalOr> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "||\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<LogicalNot> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "!\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "&\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "|\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "~\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<LessThan> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "<\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "<=\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Equal> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "==\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<NotEqual> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "!=\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<GreaterThan> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << ">\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << ">=\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<TuplePointer> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "&\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "vref " << expr->getSymbol()->name();
    }

    template<typename ValueType>
    void EnterExpressionVisitor::visitConstant(boost::shared_ptr<const Expression<Reference<Constant<ValueType> > > > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "cref ";
      TypeNameFlow typePrinter(out);
      expr->getSymbol()->type()->accept(typePrinter);
      out << ' ' << expr->getSymbol()->valueString();
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<Base> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int8_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint8_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int16_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint16_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int32_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint32_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::int64_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::uint64_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<float> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<double> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Constant<std::string> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Function> > >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "fref " << expr->getSymbol()->name();
    }

    void EnterExpressionVisitor::visit(ptr<Expression<Reference<Tuple> > >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      Indent ind = attributeManager.getInheritedAttribute().indent();

      out << indent(ind) << "tref\n";
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + IndentFactor, out));
    }

    void LeaveExpressionVisitor::visit(ptr<Expression<Base> >::const_type expr)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out() << "\n";
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    void LeaveExpressionVisitor::visit(ptr<InnerExpression>::const_type expr)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out() << "\n";
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    void PrintDefSymbolFlow::visit(ptr<Symbol<Module>>::const_type sym) {
      // We only want to visit functions here since we already
      // declared module-level types and variables.
      this->doEnter(sym);
      // Visit functions
      for(Symbol<Module>::ConstFunctionIterator f = sym->functionBegin(),
            fend = sym->functionEnd();
          f != fend;
          /* NULL */) {
        if (!(*f)->statementEmpty()) {
          this->doBefore(sym, f);
          (*f)->accept(*this);
          this->doAfter(sym, f);
        }
        Symbol<Module>::ConstFunctionIterator prev = f;
        if (++f != fend) {
          if (!(*prev)->statementEmpty() && !(*f)->statementEmpty()) {
            this->doBetween(sym, prev, f);
          }
        }
      }
      this->doLeave(sym);
    }
  }

  void print(Printer::Stream &out, ptr<Node<Base> >::const_type node)
  {
    if (ptr<Symbol<Module> >::const_type s =
        boost::dynamic_pointer_cast<const Symbol<Module> >(node)) {
      ptr<ConstSymbolVisitor>::type declflow(new Printer::PrintDeclSymbolFlow(out));
      s->accept(*declflow);
      ptr<ConstSymbolVisitor>::type defflow(new Printer::PrintDefSymbolFlow(out));
      s->accept(*defflow);
    }
    else if (ptr<Symbol<Type<TypeBase> > >::const_type t =
             boost::dynamic_pointer_cast<const Symbol<Type<TypeBase> > >(node)) {
      ptr<ConstSymbolVisitor>::type flow(new Printer::TypeNameFlow(out));
      t->accept(*flow);
    }
    else if (ptr<Symbol<Base> >::const_type s =
             boost::dynamic_pointer_cast<const Symbol<Base> >(node)) {
      ptr<ConstSymbolVisitor>::type defflow(new Printer::PrintDefSymbolFlow(out));
      s->accept(*defflow);
    }
    else if (ptr<Statement<Base> >::const_type s =
             boost::dynamic_pointer_cast<const Statement<Base> >(node)) {
      ptr<ConstStatementVisitor>::type flow(new Printer::PrintFlow(out));
      s->accept(*flow);
    }
    else if (ptr<Expression<Base> >::const_type e =
             boost::dynamic_pointer_cast<const Expression<Base> >(node)) {
      ptr<ConstExpressionVisitor>::type flow(new Printer::PrintExpressionFlow(out));
      e->accept(*flow);
    }
  }
}
