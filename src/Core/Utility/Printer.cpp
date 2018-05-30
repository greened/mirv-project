#include <mirv/Core/IR/Constants.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/Filter/AttributeFlow.hpp>
#include <mirv/Core/Filter/ForwardControlFlow.hpp>
#include <mirv/Core/Filter/FlowAction.hpp>
#include <mirv/Core/Filter/ValueFlow.hpp>
#include <mirv/Core/Filter/SymbolFlow.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Utility/Printer.hpp>
#include <mirv/Core/Utility/Debug.hpp>

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <memory>

namespace mirv {
  namespace Printer {
    namespace detail {
      /// This is the most common printer formatter.  It constructs
      /// rather verbose output that is easier to read than that from
      /// the CompactFormatter.  The Printer uses it to output general
      /// IR.
      class DefaultFormatter {
      public:
        /// Do something after printing an expression operator.
        class Operator {
        public:
          Operator(Producer & expr) {}
          Stream &operator()(Stream &out) const { return out; }
        };

        /// Terminate the line.
        class Newline {
        public:
          Stream &operator()(Stream &out) const {
            return out << '\n';
          }
        };

        /// Output a delimiter between IR constructs.  The
        /// DefaultFormatter does not use delimiters.
        class Delimit {
        public:
          Stream &operator()(Stream &out) const {
            return out;
          }
        };

        /// Represent the curent indent amount.
        class Indent {
        public:
          Indent(int ind) : val(ind) {};
          Stream &operator()(Stream &out) const {
            checkInvariant(val >= 0, "Indent underflow");
            int i = val;
            while(i--) {
              out << ' ';
            }
            return out;
          }

        private:
          /// The number of spaces to print.
          int val;
        };

        /// The factor by which to increase the indent at deeper nesting
        /// levels.
        static int indentFactor(void) {
          return 3;
        }
      };

      /// This is a formatter that outputs IR in a very compact
      /// representation.  The printer uses it primarity to generate
      /// type names and constant initializers.
      class CompactFormatter {
      public:
        /// Do something after printing an expression operator.
        class Operator {
        public:
          Operator(Producer & expr) {}
          Stream &operator()(Stream &out) const { return out; }
        };

        /// This is a function class to output a newline.  For the
        /// compact formatter, do nothing.  This causes all output to
        /// be on a single line.
        class Newline {
        public:
          Stream &operator()(Stream &out) const {
            return out;
          }
        };

        /// Output a delimeter.  This does nothing for the compact
        /// formatter.
        class Delimit {
        public:
          Stream &operator()(Stream &out) const {
            return out;
          }
        };

        /// This is an I/O manipulator to output whitespace to place
        /// the output point at the current indent level.
        class Indent {
        public:
          Indent(int ind) : val(ind) {};
          Stream &operator()(Stream &out) const {
            checkInvariant(val >= 0, "Indent underflow");
            if (val > 0) {
              out << ' ';
            }
            return out;
          }

        private:
          /// The number of spaces to print.
          int val;
        };

        /// The factor by which to increase the indent at deeper nesting
        /// levels.
        static int indentFactor(void) {
          return 1;
        }
      };
    }

    /// This is a functor to annotate operators.  Delegate to the
    /// operator functor of the particular formatter in use.
    template<typename Formatter>
    class Operator : public Formatter::Operator {
    public:
      Operator(Producer & expr)
          : Formatter::Operator(expr) {}
    };

    /// This is a functor to output newlines.  Delegate to the newline
    /// functor of the particular formatter in use.
    template<typename Formatter>
    class Newline : public Formatter::Newline {};

    /// This is a functor to output delimiters.  Delegate to the
    /// delimiter of the particular formatter in use.
    template<typename Formatter>
    class Delimit : public Formatter::Delimit {};

    /// Represent the current indent level.  Delegate to the indent
    /// representation of the particular formatter in use.
    template<typename Formatter>
    class Indent : public Formatter::Indent {
    public:
      Indent(int ind) : Formatter::Indent(ind) {}
    };

    template<typename Formatter>
    inline Stream &
    operator<<(Stream &out,
               const Operator<Formatter> &formatter) {
      return formatter(out);
    }

    template<typename Formatter>
    inline Stream &
    operator<<(Stream &out,
               const Newline<Formatter> &formatter) {
      return formatter(out);
    }

    template<typename Formatter>
    inline Stream &
    operator<<(Stream &out,
               const Delimit<Formatter> &formatter) {
      return formatter(out);
    }

    template<typename Formatter>
    inline Stream &
    operator<<(Stream &out,
               const Indent<Formatter> &formatter) {
      return formatter(out);
    }

    namespace {
      template<typename Formatter = Printer::detail::DefaultFormatter>
      void printImpl(Stream &out, ptr<Module> module);
      template<typename Formatter = Printer::detail::DefaultFormatter>
      void printImpl(Stream &out, ptr<Function> function);
      template<typename Formatter = Printer::detail::DefaultFormatter>
      void printImpl(Stream &out, ptr<const Type> symbol);
      template<typename Formatter = Printer::detail::DefaultFormatter>
      void printImpl(Stream &out, ptr<Control> statement);
      template<typename Formatter = Printer::detail::DefaultFormatter>
      void printImpl(Stream &out, ptr<Producer> expression);
    }

    /// Define the inherited attibute.
    class InheritedAttribute {
    private:
      /// The current indent level.
      int ind;
      /// The stream to dump to.
      Stream *ot;

    public:
      InheritedAttribute(void) = default;

      InheritedAttribute(int i, Stream &o) : ind(i), ot(&o) {}

      int indent(void) const {
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
    template<typename Formatter>
    class EnterDeclSymbolVisitor : public SymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterDeclSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(Module & sym) override;
    };

    /// Invoke the EnterDeclSymbolVisitor upon entry to a symbol
    /// declaration.
    template<typename Formatter>
    class EnterDeclSymbolAction :
      public VisitAction<EnterDeclSymbolVisitor<Formatter>> {
    public:
      EnterDeclSymbolAction(FlowAttributeManagerType &attributeManager) :
        VisitAction<EnterDeclSymbolVisitor<Formatter>>(attributeManager) {}
    };

    /// Perform preorder traversal actions upon entering symbol
    /// definitions.
    template<typename Formatter>
    class EnterDefSymbolVisitor : public SymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterDefSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(Module & sym) override;
    };

    /// Invoke the EnterDefSymbolVisitor upon entry to a symbol
    /// definition.
    template<typename Formatter>
    class EnterDefSymbolAction :
      public VisitAction<EnterDefSymbolVisitor<Formatter>> {
    public:
      EnterDefSymbolAction(FlowAttributeManagerType &attributeManager) :
        VisitAction<EnterDefSymbolVisitor<Formatter>>(attributeManager) {}
    };

    /// Perform preorder traversal actions upon exiting symbol
    /// definitions.
    template<typename Formatter>
    class LeaveDefSymbolVisitor : public SymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveDefSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(Module & sym) override;
    };

    /// Invoke the EnterDefSymbolVisitor upon entry to a symbol
    /// definition.
    template<typename Formatter>
    class LeaveDefSymbolAction :
      public VisitAction<LeaveDefSymbolVisitor<Formatter>> {
    public:
      LeaveDefSymbolAction(FlowAttributeManagerType &attributeManager) :
        VisitAction<LeaveDefSymbolVisitor<Formatter>>(attributeManager) {}
    };

    /// Leaving each symbol declaration.
    template<typename Formatter>
    class LeaveDeclSymbolVisitor : public SymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveDeclSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      /// Print the final newline after each symbol declaration.
      void visit(Symbol &) override;

      /// Do nothing for most types.
      void visit(const Type &) override {};

      /// Print the final newline after tuple types.
      void visit(const TupleType &) override;
    };

    /// Invoke the LeaveDeclSymbolVisitor upon exiting a Symbol.
    template<typename Formatter>
    class LeaveDeclSymbolAction :
      public VisitAction<LeaveDeclSymbolVisitor<Formatter>> {
    public:
      LeaveDeclSymbolAction(FlowAttributeManagerType &attributeManager) :
        VisitAction<LeaveDeclSymbolVisitor<Formatter>>(attributeManager) {}
    };

    /// Perform preorder traversal actions upon enteringglobal
    /// definitions.
    template<typename Formatter>
    class EnterDefGlobalVisitor : public ValueVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterDefGlobalVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(Function & sym) override;
    };

    /// Invoke the EnterDefSymbolVisitor upon entry to a symbol
    /// definition.
    template<typename Formatter>
    class EnterDefGlobalAction :
      public VisitAction<EnterDefGlobalVisitor<Formatter>> {
    public:
      EnterDefGlobalAction(FlowAttributeManagerType &attributeManager) :
        VisitAction<EnterDefGlobalVisitor<Formatter>>(attributeManager) {}
    };

    /// Leaving each symbol definition.
    template<typename Formatter>
    class LeaveDefGlobalVisitor : public ValueVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveDefGlobalVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      /// Print the final newline after each global definition.
      void visit(Function & sym) override;
    };

    /// Invoke the LeaveDefGlobalVisitor upon leaving the definition
    /// of a global.
    template<typename Formatter>
    class LeaveDefGlobalAction :
      public VisitAction<LeaveDefGlobalVisitor<Formatter>> {
    public:
      LeaveDefGlobalAction(FlowAttributeManagerType &attributeManager) :
        VisitAction<LeaveDefGlobalVisitor<Formatter>>(attributeManager) {}
    };

    /// Entering each statement
    template<typename Formatter>
    class EnterControlVisitor : public ControlVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterControlVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(Block & stmt) override;
      void visit(CIBlock & stmt) override;
      void visit(IfThen & stmt) override;
      void visit(IfElse & stmt) override;
      void visit(DoWhile & stmt) override;
      void visit(Switch & stmt) override;
      void visit(Case & stmt) override;
      void visit(Goto & stmt) override;
      void visit(Return & stmt) override;
      void visit(Sequence & stmt) override;
    };

    /// Invoke the EnterControlVisitor upon entry to a statement.
    template<typename Formatter>
    class EnterControlAction :
      public VisitAction<EnterControlVisitor<Formatter>> {
    public:
      EnterControlAction(FlowAttributeManagerType &attributeManager) :
        VisitAction<EnterControlVisitor<Formatter>>(attributeManager) {}
    };

    /// Leaving each statement
    template<typename Formatter>
    class LeaveControlVisitor : public ControlVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveControlVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(Control & stmt) override;
      void visit(Block & stmt) override;
      void visit(Return & stmt) override;
    };

    /// Invoke the LeaveControlVisitor when exiting statements.
    template<typename Formatter>
    class LeaveControlAction :
      public VisitAction<LeaveControlVisitor<Formatter>> {
    public:
      LeaveControlAction(FlowAttributeManagerType &attributeManager) :
        VisitAction<LeaveControlVisitor<Formatter>>(attributeManager) {}
    };

    /// Entering each expression
    template<typename Formatter>
    class EnterValueVisitor : public ValueVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterValueVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(Function & stmt) override;
      void visit(GlobalVariable & stmt) override;
      void visit(Allocate & stmt) override;
      void visit(Add & expr) override;
      void visit(Subtract & expr) override;
      void visit(Multiply & expr) override;
      void visit(Divide & expr) override;
      void visit(Modulus & expr) override;
      void visit(Negate & expr) override;
      void visit(LogicalAnd & expr) override;
      void visit(LogicalOr & expr) override;
      void visit(LogicalNot & expr) override;
      void visit(BitwiseAnd & expr) override;
      void visit(BitwiseOr & expr) override;
      void visit(BitwiseComplement & expr) override;
      void visit(LessThan & expr) override;
      void visit(LessThanOrEqual & expr) override;
      void visit(Equal & expr) override;
      void visit(NotEqual & expr) override;
      void visit(GreaterThan & expr) override;
      void visit(GreaterThanOrEqual & expr) override;
      void visit(TuplePointer & expr) override;
      void visit(Load & expr) override;
      void visit(Phi & stmt) override;
      void visit(Store & stmt) override;
      void visit(FunctionCall & stmt) override;
      void visit(PureFunctionCall & stmt) override;
      void visit(IntegerConstant & expr) override;
      void visit(FloatingPointConstant & expr) override;
      void visit(TupleConstant & expr) override;
    };

    /// Invoke the EnterValueVisitor upon entry to an expression.
    template<typename Formatter>
    class EnterProducerAction :
      public VisitAction<EnterValueVisitor<Formatter>> {
    public:
      EnterProducerAction(FlowAttributeManagerType &attributeManager) :
        VisitAction<EnterValueVisitor<Formatter>>(attributeManager) {}
    };

    /// Leaving each expression
    template<typename Formatter>
    class LeaveValueVisitor : public ValueVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveValueVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(Producer & expr) override;
    };

    /// Invoke the LeaveValueVisitor upon leaving an expression.
    template<typename Formatter>
    class LeaveProducerAction :
      public VisitAction<LeaveValueVisitor<Formatter>> {
    public:
      LeaveProducerAction(FlowAttributeManagerType &attributeManager) :
        VisitAction<LeaveValueVisitor<Formatter>>(attributeManager) {}
    };

    /// This is the flow to print expressions.
    template<typename Formatter>
    class PrintValueFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardValueFlowGenerator,
      EnterProducerAction<Formatter>,
      LeaveProducerAction<Formatter>,
      NullAction,
      NullAction,
      NullAction
      > {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardValueFlowGenerator,
      EnterProducerAction<Formatter>,
      LeaveProducerAction<Formatter>,
      NullAction,
      NullAction,
      NullAction
      > BaseType;

    public:
      PrintValueFlow(Stream &out)
          : BaseType(InheritedAttribute(0, out),
                     SynthesizedAttribute()) {}

      PrintValueFlow(FlowAttributeManagerType &)
          : BaseType(InheritedAttribute(),
                     SynthesizedAttribute()) {}

    };

    /// This is the flow to print statements.
    template<typename Formatter>
    class PrintFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardFlowGenerator,
      EnterControlAction<Formatter>,
      LeaveControlAction<Formatter>,
      NullAction,
      NullAction,
      NullAction,
      NullJoinAction,
      NullAction,
      NullAction,
      FlowAction<PrintFlow<Formatter>, PrintValueFlow<Formatter>>,
      NullAction
      > {
      typedef AttributeFlow<
        InheritedAttribute,
        SynthesizedAttribute,
        ForwardFlowGenerator,
        EnterControlAction<Formatter>,
        LeaveControlAction<Formatter>,
        NullAction,
        NullAction,
        NullAction,
        NullJoinAction,
        NullAction,
        NullAction,
        FlowAction<PrintFlow<Formatter>, PrintValueFlow<Formatter>>,
        NullAction
        > BaseType;

    public:
      PrintFlow(Stream &out) : BaseType(InheritedAttribute(0, out),
                                        SynthesizedAttribute()) {
        this->expression().setParentFlow(this);
      }

      PrintFlow(FlowAttributeManagerType &)
          : BaseType(InheritedAttribute(),
                     SynthesizedAttribute()) {
        this->expression().setParentFlow(this);
      }
    };

    /// This is the flow to print symbol declarations.  It prints the
    /// module definition since there is no such thing as a module
    /// declaration.
    template<typename Formatter>
    class PrintDeclSymbolFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      SymbolFlowGenerator,
      EnterDeclSymbolAction<Formatter>,
      LeaveDeclSymbolAction<Formatter>,
      NullAction,
      NullAction,
      NullAction,
      NullAction> {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      SymbolFlowGenerator,
      EnterDeclSymbolAction<Formatter>,
      LeaveDeclSymbolAction<Formatter>,
      NullAction,
      NullAction,
      NullAction,
      NullAction> BaseType;

    public:
      PrintDeclSymbolFlow(Stream &out)
          : BaseType(InheritedAttribute(0, out),
                     SynthesizedAttribute()) {}

      // We just want the function declaration at this point so don't
      // visit children.
      void visit(Function & sym) {
        this->doEnter(sym);
        this->doLeave(sym);
      }
    };

    /// This is the flow to print symbol defintions.
    template<typename Formatter>
    class PrintDefSymbolFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      SymbolFlowGenerator,
      EnterDefSymbolAction<Formatter>,
      LeaveDefSymbolAction<Formatter>,
      NullAction,
      NullAction,
      NullAction,
      FlowAction<PrintDefSymbolFlow<Formatter>, PrintFlow<Formatter>>
      > {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      SymbolFlowGenerator,
      EnterDefSymbolAction<Formatter>,
      LeaveDefSymbolAction<Formatter>,
      NullAction,
      NullAction,
      NullAction,
      FlowAction<PrintDefSymbolFlow<Formatter>, PrintFlow<Formatter>>
      > BaseType;

    public:
      PrintDefSymbolFlow(Stream &out, int i = 0)
          : BaseType(InheritedAttribute(i, out),
                     SynthesizedAttribute()) {
        //this->statement().setParentFlow(this);
      }

      /// We only want to visit functions here since we already
      /// declared module-level types and variables.
      void visit(Module & sym) override;
    };

    /// This is the flow to print global defintions.
    template<typename Formatter>
    class PrintDefGlobalFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardValueFlowGenerator,
      EnterDefGlobalAction<Formatter>,
      LeaveDefGlobalAction<Formatter>,
      NullAction,
      NullAction,
      NullAction//,
      //FlowAction<PrintDefGlobalFlow<Formatter>, PrintFlow<Formatter>>
      > {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardValueFlowGenerator,
      EnterDefGlobalAction<Formatter>,
      LeaveDefGlobalAction<Formatter>,
      NullAction,
      NullAction,
      NullAction//,
      //FlowAction<PrintDefGlobalFlow<Formatter>, PrintFlow<Formatter>>
      > BaseType;

    public:
      PrintDefGlobalFlow(Stream &out, int i = 0)
          : BaseType(InheritedAttribute(i, out),
                     SynthesizedAttribute()) {
        // this->statement().setParentFlow(this);
      }

      using BaseType::visit;

      /// We only want to visit functions here since we already
      /// declared module-level types and variables.
      void visit(Function & sym) override;
    };

    namespace {
      template<typename Formatter>
      void printImpl(Printer::Stream &out, ptr<Module> symbol) {
        if (symbol == nullptr) {
          out << "<null\n";
        }

        std::unique_ptr<SymbolVisitor> declflow(new Printer::PrintDeclSymbolFlow<Formatter>(out));
        symbol->accept(*declflow);
        std::unique_ptr<SymbolVisitor> defflow(new Printer::PrintDefSymbolFlow<Formatter>(out));
        symbol->accept(*defflow);
      }

      template<typename Formatter>
      void printImpl(Printer::Stream &out, ptr<Function> symbol) {
        if (symbol == nullptr) {
          out << "<null\n";
        }

        std::unique_ptr<ValueVisitor> defflow(new Printer::PrintDefGlobalFlow<Formatter>(out));
        symbol->accept(*defflow);
      }

      template<typename Formatter>
      void printImpl(Printer::Stream &out, ptr<const Type> symbol) {
        if (symbol == nullptr) {
          out << "<null\n";
        }

        out << symbol->getName();
      }

      template<typename Formatter>
      void printImpl(Printer::Stream &out, ptr<Control> statement) {
        if (statement == nullptr) {
          out << "<null\n";
        }

        std::unique_ptr<ControlVisitor> flow(new Printer::PrintFlow<Formatter>(out));
        statement->accept(*flow);
      }

      template<typename Formatter>
      void printImpl(Printer::Stream &out, ptr<Producer> expression) {
        if (expression == nullptr) {
          out << "<null\n";
        }

        std::unique_ptr<ValueVisitor> flow(new Printer::PrintValueFlow<Formatter>(out));
        expression->accept(*flow);
      }
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(Module & sym) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      attributeManager.setInheritedAttribute(InheritedAttribute(ind, out));
    }

    /// Print the final newline after each symbol declaration.
    template<typename Formatter>
    void LeaveDeclSymbolVisitor<Formatter>::visit(Symbol &) {
      if (!attributeManager.setLastSynthesizedAttribute() ||
          !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveDeclSymbolVisitor<Formatter>::visit(const TupleType &) {
      if (!attributeManager.setLastSynthesizedAttribute() ||
          !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void EnterDefSymbolVisitor<Formatter>::visit(Module & sym) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "mdef " << sym.getName() << " {"
        << Newline<Formatter>();

      ind += Formatter::indentFactor();

      // Declare global variables.
      for (auto G : sym.Globals()) {
        out << Indent<Formatter>(ind) << "gvdecl " << G->getName() << " "
            << G->ValueType()->getName();
        if (G->getInitializer()) {
          out << ' ';
          printImpl<detail::CompactFormatter>(out, G->getInitializer());
        }
        out << Newline<Formatter>();
      }

      // Declare functions.
      for (auto F : sym.Functions()) {
        out << Indent<Formatter>(ind) << "fdecl " << F->getName() << ' '
            << F->getType()->getName() << Newline<Formatter>();
      }

      // Define functions.
      PrintDefGlobalFlow<Formatter> FunctionDefFlow(out, ind);
      for (auto F : sym.Functions()) {
        F->accept(FunctionDefFlow);
      }

      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind, out));
    }

    template<typename Formatter>
    void LeaveDefSymbolVisitor<Formatter>::visit(Module & sym) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "}" << Newline<Formatter>();
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }


    template<typename Formatter>
    void EnterDefGlobalVisitor<Formatter>::visit(Function & sym) {
      if (sym.getBlock()) {
        Stream &out = attributeManager.getInheritedAttribute().out();
        int ind = attributeManager.getInheritedAttribute().indent();

        out << Indent<Formatter>(ind) << "fdef " << sym.getName()
          << Newline<Formatter>();
        attributeManager.setInheritedAttribute(
          InheritedAttribute(ind + Formatter::indentFactor(), out));

        std::unique_ptr<ControlVisitor> flow(new Printer::PrintFlow<Formatter>(out));
        sym.getBlock()->accept(*flow);
      }
    }

    template<typename Formatter>
    void LeaveDefGlobalVisitor<Formatter>::visit(Function & sym) {
      if (sym.getBlock()) {
        Stream &out = attributeManager.getInheritedAttribute().out();

        if (!attributeManager.setLastSynthesizedAttribute() ||
            !attributeManager.getLastSynthesizedAttribute().justLeft()) {
          out << Newline<Formatter>();
        }
        attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
      }
    }

    template<typename Formatter>
    void EnterControlVisitor<Formatter>::visit(Block & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "block {" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterControlVisitor<Formatter>::visit(CIBlock & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "CIBlock {" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterControlVisitor<Formatter>::visit(IfThen & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "ifThen" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterControlVisitor<Formatter>::visit(IfElse & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "ifElse" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterControlVisitor<Formatter>::visit(DoWhile & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "doWhile" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterControlVisitor<Formatter>::visit(Switch & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "switch" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterControlVisitor<Formatter>::visit(Case & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "case";
    }

    template<typename Formatter>
    void EnterControlVisitor<Formatter>::visit(Goto & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "goto ";
    }

    template<typename Formatter>
    void EnterControlVisitor<Formatter>::visit(Return & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "return" << Newline<Formatter>();
    }

    template<typename Formatter>
    void EnterControlVisitor<Formatter>::visit(Sequence & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "sequence" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void LeaveControlVisitor<Formatter>::visit(Control & stmt) {
      if (!attributeManager.setLastSynthesizedAttribute() ||
          !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveControlVisitor<Formatter>::visit(Block & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      if (!attributeManager.setLastSynthesizedAttribute() ||
          !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << Newline<Formatter>();
      }
      out << Indent<Formatter>(ind) << '}' << Newline<Formatter>();
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveControlVisitor<Formatter>::visit(Return & stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();

      if (!attributeManager.setLastSynthesizedAttribute() ||
          !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Function & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << expr.getName() << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(GlobalVariable & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "global " << expr.getName() << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Allocate & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] allocate "
          << Operator<Formatter>(expr)
          << expr.getName() << ' ' << expr.ValueType()->getName()
          << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Add & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] +"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Subtract & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] -"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Multiply & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] *"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Divide & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] /"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Modulus & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] %"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Negate & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] neg"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(LogicalAnd & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] &&"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(LogicalOr & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] ||"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(LogicalNot & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] !"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(BitwiseAnd & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] &"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(BitwiseOr & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] |"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(BitwiseComplement & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] ~"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(LessThan & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] <"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(LessThanOrEqual & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] <="
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Equal & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] =="
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(NotEqual & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] !="
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(GreaterThan & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] >"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(GreaterThanOrEqual & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] >="
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(TuplePointer & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] &"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Load & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] load"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Phi & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] phi"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(Store & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] store"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(FunctionCall & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] fcall"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(PureFunctionCall & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] pfcall"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(IntegerConstant & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] intconst"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(FloatingPointConstant & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] fpconst"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterValueVisitor<Formatter>::visit(TupleConstant & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '[' << expr.getName() << "] tupleconst"
          << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void LeaveValueVisitor<Formatter>::visit(Producer & expr) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      if (!attributeManager.setLastSynthesizedAttribute() ||
          !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void PrintDefSymbolFlow<Formatter>::visit(Module & sym) {
      this->doEnter(sym);
      this->doLeave(sym);
    }

    template<typename Formatter>
    void PrintDefGlobalFlow<Formatter>::visit(Function & sym) {
      this->doEnter(sym);
      this->doLeave(sym);
    }
  }

  namespace Printer {
    namespace detail {
      /// This is a debugging formatter to annotate expressions with
      /// their types.  Error reporting uses this to give useful
      /// information in the case of validation failures.
      class TypeFormatter : public DefaultFormatter {
      public:
        /// Print the type of expression after its operator.
        class Operator {
        private:
          Producer &expression;

        public:
          Operator(Producer &expr)
              : expression(expr) {}
          Stream &operator()(Stream &out) const {
            out << ' ' << expression.getType()->getName() << ' ';
            return out;
          }
        };
      };
    }
  }

  void print(Printer::Stream &out, ptr<Module> module) {
    Printer::printImpl(out, module);
  }

  void print(Printer::Stream &out, ptr<Function> function) {
    Printer::printImpl(out, function);
  }

  void print(Printer::Stream &out, ptr<Control> control) {
    Printer::printImpl(out, control);
  }

  void print(Printer::Stream &out, ptr<const Type> type) {
    Printer::printImpl(out, type);
  }

  void print(Printer::Stream &out, ptr<Producer> expression) {
    Printer::printImpl(out, expression);
  }

  void printWithTypes(Printer::Stream &out, ptr<Control> statement) {
    Printer::printImpl(out, statement);
  }

  void printWithTypes(Printer::Stream &out, ptr<const Type> type) {
    Printer::printImpl<Printer::detail::TypeFormatter>(out, type);
  }

  void printWithTypes(Printer::Stream &out,
                      ptr<Producer> expression) {
    Printer::printImpl<Printer::detail::TypeFormatter>(out, expression);
  }
}
