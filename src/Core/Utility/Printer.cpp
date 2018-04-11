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

#include <boost/algorithm/string.hpp>
#include <iostream>

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
          Operator(ptr<const Expression<Base> > expr) {}
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
          Operator(ptr<const Expression<Base> > expr) {}
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
      Operator(ptr<const Expression<Base> > expr) 
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
               const Operator<Formatter> &formatter) 
    {
      return formatter(out);
    }

    template<typename Formatter>
    inline Stream &
    operator<<(Stream &out,
               const Newline<Formatter> &formatter) 
    {
      return formatter(out);
    }

    template<typename Formatter>
    inline Stream &
    operator<<(Stream &out,
               const Delimit<Formatter> &formatter) 
    {
      return formatter(out);
    }

    template<typename Formatter>
    inline Stream &
    operator<<(Stream &out,
               const Indent<Formatter> &formatter) 
    {
      return formatter(out);
    }

    namespace {
      template<typename Formatter = Printer::detail::DefaultFormatter>
      void printImpl(Stream &out, ptr<const Node<Base> > node);
    }

    typedef std::vector<ptr<const Symbol<Type<TypeBase> > >> TypeList;

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
      TypeNameInheritedAttribute(void) = default;

      TypeNameInheritedAttribute(Stream &o, TypeList &t)
          : ot(&o), ts(&t) {}

      Stream &out(void) const {
        checkInvariant(ot != 0, "No output stream");
        return *ot;
      }

      void typeStackPushBack(ptr<const Symbol<Type<TypeBase> > > type) const {
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
      typeStackFind(ptr<const Symbol<Type<TypeBase> > > type) const {
        return std::find(typeStackBegin(), typeStackEnd(), type);
      }
    };

    /// Define the synthesized attribute.
    class TypeNameSynthesizedAttribute {
    private:
      TypeList *ts;

    public:
      TypeNameSynthesizedAttribute(void) = default;

      TypeNameSynthesizedAttribute(TypeList &t) : ts(&t) {}
    };

    typedef FlowAttributeManager<
      TypeNameInheritedAttribute,
      TypeNameSynthesizedAttribute
      > TypeNameFlowAttributeManagerType;

    /// Type name construction.
    template<typename Formatter>
    class EnterTypeNameVisitor : public ConstSymbolVisitor {
    private:
      TypeNameFlowAttributeManagerType &attributeManager;

    public:
      EnterTypeNameVisitor(TypeNameFlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Symbol<Type<Integral> > >);
      void visit(ptr<const Symbol<Type<Floating> > >);
      void visit(ptr<const Symbol<Type<Tuple> > >);
      void visit(ptr<const Symbol<Type<Derived> > >);
    };

    /// Apply the EnterTypeNameVisitor to type symbols upon entry.
    template<typename Formatter>
    class EnterTypeNameAction : public VisitAction<EnterTypeNameVisitor<Formatter>> {
    public:
      EnterTypeNameAction(TypeNameFlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterTypeNameVisitor<Formatter>>(attributeManager) {}
    };

    /// Perform preorder traversal actions before visiting child
    /// types.
    template<typename Formatter>
    class BeforeTypeNameVisitor : public ConstSymbolVisitor {
    private:
      TypeNameFlowAttributeManagerType &attributeManager;

    public:
      BeforeTypeNameVisitor(TypeNameFlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Symbol<Type<TypeBase> > >);
      void visit(ptr<const Symbol<Type<Derived> > >);
    };

    template<typename Formatter>
    class BeforeTypeNameAction : public VisitAction<BeforeTypeNameVisitor<Formatter>> {
    public:
      BeforeTypeNameAction(TypeNameFlowAttributeManagerType &attributeManager) 
          : VisitAction<BeforeTypeNameVisitor<Formatter>>(attributeManager) {}
    };

    /// Perform inorder traversal actions after visiting child
    /// types.
    template<typename Formatter>
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
      void operator()(ptr<const InnerType> parent,
                      InputIterator node) {
        Stream &out = attributeManager.getInheritedAttribute().out();
        if (ptr<const Symbol<Type<FunctionType> > > function =
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

    /// Perform inorder traversal actions in between visits of child
    /// types.
    template<typename Formatter>
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
        if (ptr<const Symbol<Type<FunctionType> > > function =
            dyn_cast<const Symbol<Type<FunctionType> > >(parent)) {
          if (node == function->begin()) {
            return;
          }
        }
        out << ", ";
      }
    };

    /// Perform post-order traversal leave actions when exiting a
    /// symbol.
    template<typename Formatter>
    class LeaveTypeNameVisitor : public ConstSymbolVisitor {
    private:
      TypeNameFlowAttributeManagerType &attributeManager;

    public:
      LeaveTypeNameVisitor(TypeNameFlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Symbol<Type<Pointer> > >);
      void visit(ptr<const Symbol<Type<FunctionType> > >);
      void visit(ptr<const Symbol<Type<Tuple> > >);
      void visit(ptr<const Symbol<Type<Derived> > >);
    };

    /// Invoke the LeaveTypeNameVisitor upon exiting a type symbol.
    template<typename Formatter>
    class LeaveTypeNameAction : public VisitAction<LeaveTypeNameVisitor<Formatter>> {
    public:
      LeaveTypeNameAction(TypeNameFlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveTypeNameVisitor<Formatter>>(attributeManager) {}
    };

    /// This is a flow to compute the name of some type.  It handles
    /// including the names of member types.  This provides a
    /// cononical name for each type.
    template<typename Formatter>
    class TypeNameFlow : public AttributeFlow<
      TypeNameInheritedAttribute,
      TypeNameSynthesizedAttribute,
      ConstSymbolFlowGenerator,
      EnterTypeNameAction<Formatter>,
      LeaveTypeNameAction<Formatter>,
      BeforeTypeNameAction<Formatter>,
      AfterTypeNameAction<Formatter>,
      BetweenTypeNameAction<Formatter>,
      NullAction> {
    private:
      typedef AttributeFlow<
      TypeNameInheritedAttribute,
      TypeNameSynthesizedAttribute,
      ConstSymbolFlowGenerator,
      EnterTypeNameAction<Formatter>,
      LeaveTypeNameAction<Formatter>,
      BeforeTypeNameAction<Formatter>,
      AfterTypeNameAction<Formatter>,
      BetweenTypeNameAction<Formatter>,
      NullAction> BaseType;

      Printer::TypeList typeStack;

    public:
      TypeNameFlow(std::ostream &out)
          : BaseType(TypeNameInheritedAttribute(out, typeStack),
                     TypeNameSynthesizedAttribute()) {}
    };

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

    /// Type name construction.
    template<typename Formatter>
    class EnterConstantValueVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

      template<typename ConstantType>
      void
      visitPrimitiveConstant(boost::shared_ptr<const Symbol<Constant<ConstantType> > > sym) {
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << sym->value();
      }

    public:
      EnterConstantValueVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Symbol<Constant<std::int8_t> > >);
      void visit(ptr<const Symbol<Constant<std::uint8_t> > >);
      void visit(ptr<const Symbol<Constant<std::int16_t> > >);
      void visit(ptr<const Symbol<Constant<std::uint16_t> > >);
      void visit(ptr<const Symbol<Constant<std::int32_t> > >);
      void visit(ptr<const Symbol<Constant<std::uint32_t> > >);
      void visit(ptr<const Symbol<Constant<std::int64_t> > >);
      void visit(ptr<const Symbol<Constant<std::uint64_t> > >);
      void visit(ptr<const Symbol<Constant<float> > >);
      void visit(ptr<const Symbol<Constant<double> > >);
      void visit(ptr<const Symbol<Constant<std::string> > >);
      void visit(ptr<const Symbol<Constant<Address> > >);
    };

    /// Invoke the EnterConstantValueVisitor upon entry to a constant
    /// symbol.
    template<typename Formatter>
    class EnterConstantValueAction : public VisitAction<EnterConstantValueVisitor<Formatter>> {
    public:
      EnterConstantValueAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterConstantValueVisitor<Formatter>>(attributeManager) {}
    };

    /// This is a flow to compute the value of some constant.
    template<typename Formatter>
    class ConstantValueFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstSymbolFlowGenerator,
      EnterConstantValueAction<Formatter>,
      NullAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction> {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstSymbolFlowGenerator,
      EnterConstantValueAction<Formatter>,
      NullAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction> BaseType;

      Printer::TypeList typeStack;

    public:
      ConstantValueFlow(std::ostream &out)
        : BaseType(InheritedAttribute(0, out),
                   SynthesizedAttribute()) {}
    };

    /// Entering each symbol
    template<typename Formatter>
    class EnterDeclSymbolVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterDeclSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Symbol<Module> > sym);
      void visit(ptr<const Symbol<Function> > sym);
      void visit(ptr<const Symbol<Variable> > sym);
      void visit(ptr<const Symbol<GlobalVariable> > sym);
      void visit(ptr<const Symbol<Type<TypeBase> > > sym);
      void visit(ptr<const Symbol<Type<Tuple> > > sym);
    };

    /// Invoke the EnterDeclSymbolVisitor upon entry to a symbol
    /// declaration.
    template<typename Formatter>
    class EnterDeclSymbolAction : public VisitAction<EnterDeclSymbolVisitor<Formatter>> {
    public:
      EnterDeclSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterDeclSymbolVisitor<Formatter>>(attributeManager) {}
    };

    /// Perform preorder traversal actions upon entering symbol
    /// definitions.
    template<typename Formatter>
    class EnterDefSymbolVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterDefSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Symbol<Module> > sym);
      void visit(ptr<const Symbol<Function> > sym);
      void visit(ptr<const Symbol<Variable> > sym);
      void visit(ptr<const Symbol<GlobalVariable> > sym);
    };

    /// Invoke the EnterDefSymbolVisitor upon entry to a symbol
    /// definition.
    template<typename Formatter>
    class EnterDefSymbolAction : public VisitAction<EnterDefSymbolVisitor<Formatter>> {
    public:
      EnterDefSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterDefSymbolVisitor<Formatter>>(attributeManager) {}
    };

    /// Leaving each symbol declaration.
    template<typename Formatter>
    class LeaveDeclSymbolVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveDeclSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      /// Print the final newline after each symbol declaration.
      void visit(ptr<const Symbol<Base> >);

      /// Do nothing for most types.
      void visit(ptr<const Symbol<Type<TypeBase> >  >);

      /// Print the final newline after tuple types.
      void visit(ptr<const Symbol<Type<Tuple> > >);
    };

    /// Invoke the LeaveDeclSymbolVisitor upon exiting a Symbol.
    template<typename Formatter>
    class LeaveDeclSymbolAction : public VisitAction<LeaveDeclSymbolVisitor<Formatter>> {
    public:
      LeaveDeclSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveDeclSymbolVisitor<Formatter>>(attributeManager) {}
    };

    /// Leaving each symbol definition.
    template<typename Formatter>
    class LeaveDefSymbolVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveDefSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      /// Print the final newline after each symbol definition.
      void visit(ptr<const Symbol<Variable> > sym);
      void visit(ptr<const Symbol<GlobalVariable> > sym);
      void visit(ptr<const Symbol<Module> > sym);
      void visit(ptr<const Symbol<Function> > sym);
    };

    /// Invoke the LeaveDefSymbolVisitor upon leaving the definition
    /// of a symbol.
    template<typename Formatter>
    class LeaveDefSymbolAction : public VisitAction<LeaveDefSymbolVisitor<Formatter>> {
    public:
      LeaveDefSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveDefSymbolVisitor<Formatter>>(attributeManager) {}
    };

    /// Entering each statement
    template<typename Formatter>
    class EnterStatementVisitor : public ConstStatementVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterStatementVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Statement<Block> > stmt);
      void visit(ptr<const Statement<IfThen> > stmt);
      void visit(ptr<const Statement<IfElse> > stmt);
      void visit(ptr<const Statement<While> > stmt);
      void visit(ptr<const Statement<DoWhile> > stmt);
      void visit(ptr<const Statement<Switch> > stmt);
      void visit(ptr<const Statement<Case> > stmt);
      void visit(ptr<const Statement<CaseBlock> > stmt);
      void visit(ptr<const Statement<Before> > stmt);
      void visit(ptr<const Statement<After> > stmt);
      void visit(ptr<const Statement<Goto> > stmt);
      void visit(ptr<const Statement<Return> > stmt);
      void visit(ptr<const Statement<Phi> > stmt);
      void visit(ptr<const Statement<Store> > stmt);
      void visit(ptr<const Statement<Call> > stmt);
      void visit(ptr<const Statement<Allocate> > stmt);
    };

    /// Invoke the EnterStatementVisitor upon entry to a statement.
    template<typename Formatter>
    class EnterStatementAction : public VisitAction<EnterStatementVisitor<Formatter>> {
    public:
      EnterStatementAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterStatementVisitor<Formatter>>(attributeManager) {}
    };

    /// Leaving each statement
    template<typename Formatter>
    class LeaveStatementVisitor : public ConstStatementVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveStatementVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Statement<Base> > stmt);
      void visit(ptr<const Statement<Block> > stmt);
      void visit(ptr<const Statement<Return> > stmt);
    };

    /// Invoke the LeaveStatementVisitor when exiting statements.
    template<typename Formatter>
    class LeaveStatementAction : public VisitAction<LeaveStatementVisitor<Formatter>> {
    public:
      LeaveStatementAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveStatementVisitor<Formatter>>(attributeManager) {}
    };

    /// Entering each expression
    template<typename Formatter>
    class EnterExpressionVisitor : public ConstExpressionVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

      template<typename ValueType>
      void visitConstant(boost::shared_ptr<const Expression<Reference<Constant<ValueType> > > > expr);

    public:
      EnterExpressionVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Expression<Add> > expr);
      void visit(ptr<const Expression<Subtract> > expr);
      void visit(ptr<const Expression<Multiply> > expr);
      void visit(ptr<const Expression<Divide> > expr);
      void visit(ptr<const Expression<Modulus> > expr);
      void visit(ptr<const Expression<Negate> > expr);
      void visit(ptr<const Expression<LogicalAnd> > expr);
      void visit(ptr<const Expression<LogicalOr> > expr);
      void visit(ptr<const Expression<LogicalNot> > expr);
      void visit(ptr<const Expression<BitwiseAnd> > expr);
      void visit(ptr<const Expression<BitwiseOr> > expr);
      void visit(ptr<const Expression<BitwiseComplement> > expr);
      void visit(ptr<const Expression<LessThan> > expr);
      void visit(ptr<const Expression<LessThanOrEqual> > expr);
      void visit(ptr<const Expression<Equal> > expr);
      void visit(ptr<const Expression<NotEqual> > expr);
      void visit(ptr<const Expression<GreaterThan> > expr);
      void visit(ptr<const Expression<GreaterThanOrEqual> > expr);
      void visit(ptr<const Expression<TuplePointer> > expr);
      void visit(ptr<const Expression<Reference<Variable> > > expr);
      void visit(ptr<const Expression<Reference<Function> > > expr);
      void visit(ptr<const Expression<Load> > expr);
      void visit(ptr<const Expression<Reference<Constant<std::int8_t> > > > expr);
      void visit(ptr<const Expression<Reference<Constant<std::uint8_t> > > > expr);
      void visit(ptr<const Expression<Reference<Constant<std::int16_t> > > > expr);
      void visit(ptr<const Expression<Reference<Constant<std::uint16_t> > > > expr);
      void visit(ptr<const Expression<Reference<Constant<std::int32_t> > > > expr);
      void visit(ptr<const Expression<Reference<Constant<std::uint32_t> > > > expr);
      void visit(ptr<const Expression<Reference<Constant<std::int64_t> > > > expr);
      void visit(ptr<const Expression<Reference<Constant<std::uint64_t> > > > expr);
      void visit(ptr<const Expression<Reference<Constant<float> > > > expr);
      void visit(ptr<const Expression<Reference<Constant<double> > > > expr);
      void visit(ptr<const Expression<Reference<Constant<Base> > > > expr);
      void visit(ptr<const Expression<Reference<Constant<std::string> > > > expr);
    };

    /// Invoke the EnterExpressionVisitor upon entry to an expression.
    template<typename Formatter>
    class EnterExpressionAction : public VisitAction<EnterExpressionVisitor<Formatter>> {
    public:
      EnterExpressionAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterExpressionVisitor<Formatter>>(attributeManager) {}
    };

    /// Leaving each expression
    template<typename Formatter>
    class LeaveExpressionVisitor : public ConstExpressionVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveExpressionVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Expression<Base> > expr);
      void visit(ptr<const InnerExpression> expr);
    };

    /// Invoke the LeaveExpressionVisitor upon leaving an expression.
    template<typename Formatter>
    class LeaveExpressionAction : public VisitAction<LeaveExpressionVisitor<Formatter>> {
    public:
      LeaveExpressionAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveExpressionVisitor<Formatter>>(attributeManager) {}
    };

    /// This is the flow to print expressions.
    template<typename Formatter>
    class PrintExpressionFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstForwardExpressionFlowGenerator,
      EnterExpressionAction<Formatter>,
      LeaveExpressionAction<Formatter>,
      NullAction,
      NullAction,
      NullAction
      > {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstForwardExpressionFlowGenerator,
      EnterExpressionAction<Formatter>,
      LeaveExpressionAction<Formatter>,
      NullAction,
      NullAction,
      NullAction
      > BaseType;

    public:
      PrintExpressionFlow(Stream &out)
          : BaseType(InheritedAttribute(0, out),
                     SynthesizedAttribute()) {}

      PrintExpressionFlow(FlowAttributeManagerType &)
          : BaseType(InheritedAttribute(),
                     SynthesizedAttribute()) {}

    };

    /// This is the flow to print statements.
    template<typename Formatter>
    class PrintFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstForwardStatementFlowGenerator,
      EnterStatementAction<Formatter>,
      LeaveStatementAction<Formatter>,
      NullAction,
      NullAction,
      NullAction,
      NullJoinAction,
      NullAction,
      FlowAction<PrintFlow<Formatter>, PrintExpressionFlow<Formatter>>,
      NullAction
      >,
      public boost::enable_shared_from_this<PrintFlow<Formatter>> {
      typedef AttributeFlow<
        InheritedAttribute,
        SynthesizedAttribute,
        ConstForwardStatementFlowGenerator,
        EnterStatementAction<Formatter>,
        LeaveStatementAction<Formatter>,
        NullAction,
        NullAction,
        NullAction,
        NullJoinAction,
        NullAction,
        FlowAction<PrintFlow<Formatter>, PrintExpressionFlow<Formatter>>,
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

      // We need to reverse the order in which we visit the
      // assignment operands.
      void visit(ptr<const Statement<Store> > stmt) {
        this->doEnter(stmt);

        for (auto i = stmt->expressionBegin();
             i != stmt->expressionEnd(); ++i) {
          this->doBeforeExpression(stmt, i);
          this->doExpression(stmt, i);
          this->doAfterExpression(stmt, i);
        }

        this->doLeave(stmt);
      }

      // We need to reverse the order in which we visit the
      // allocate operands.
      void visit(ptr<const Statement<Allocate> > stmt) {
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
    template<typename Formatter>
    class PrintDeclSymbolFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstSymbolFlowGenerator,
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
      ConstSymbolFlowGenerator,
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
      void visit(ptr<const Symbol<Function> > sym) {
        this->doEnter(sym);
        this->doLeave(sym);
      }
    };

    /// This is the flow to print symbol defintions.
    template<typename Formatter>
    class PrintDefSymbolFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstSymbolFlowGenerator,
      EnterDefSymbolAction<Formatter>,
      LeaveDefSymbolAction<Formatter>,
      NullAction,
      NullAction,
      NullAction,
      FlowAction<PrintDefSymbolFlow<Formatter>, PrintFlow<Formatter>>
      >, public boost::enable_shared_from_this<PrintDefSymbolFlow<Formatter>> {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstSymbolFlowGenerator,
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
        this->statement().setParentFlow(this);
      }

      /// We only want to visit functions here since we already
      /// declared module-level types and variables.
      void visit(ptr<const Symbol<Module> > sym);
    };

    namespace {
      template<typename Formatter = Printer::detail::DefaultFormatter>
      void printImpl(Printer::Stream &out, ptr<const Node<Base> > node)
      {
        if (ptr<const Symbol<Module> > s =
            boost::dynamic_pointer_cast<const Symbol<Module> >(node)) {
          ptr<ConstSymbolVisitor> declflow(new Printer::PrintDeclSymbolFlow<Formatter>(out));
          s->accept(*declflow);
          ptr<ConstSymbolVisitor> defflow(new Printer::PrintDefSymbolFlow<Formatter>(out));
          s->accept(*defflow);
        }
        else if (ptr<const Symbol<Type<TypeBase> > > t =
                 boost::dynamic_pointer_cast<const Symbol<Type<TypeBase> > >(node)) {
          ptr<ConstSymbolVisitor> flow(new Printer::TypeNameFlow<Formatter>(out));
          t->accept(*flow);
        }
        else if (ptr<const Symbol<Base> > s =
                 boost::dynamic_pointer_cast<const Symbol<Base> >(node)) {
          ptr<ConstSymbolVisitor> defflow(new Printer::PrintDefSymbolFlow<Formatter>(out));
          s->accept(*defflow);
        }
        else if (ptr<const Statement<Base> > s =
                 boost::dynamic_pointer_cast<const Statement<Base> >(node)) {
          ptr<ConstStatementVisitor> flow(new Printer::PrintFlow<Formatter>(out));
          s->accept(*flow);
        }
        else if (ptr<const Expression<Base> > e =
                 boost::dynamic_pointer_cast<const Expression<Base> >(node)) {
          ptr<ConstExpressionVisitor> flow(new Printer::PrintExpressionFlow<Formatter>(out));
          e->accept(*flow);
        }
      }
    }

    template<typename Formatter>
    void EnterTypeNameVisitor<Formatter>::visit(ptr<const Symbol<Type<Integral> > > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << "int" << sym->integerBitSize();
    }

    template<typename Formatter>
    void EnterTypeNameVisitor<Formatter>::visit(ptr<const Symbol<Type<Floating> > > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << "float" << sym->integerBitSize();
    }

    template<typename Formatter>
    void EnterTypeNameVisitor<Formatter>::visit(ptr<const Symbol<Type<Tuple> > > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << "(";
      if (sym->isUniform()) {
        printImpl<detail::CompactFormatter>(out, sym->count());
        out << " x ";
      }
      attributeManager.getInheritedAttribute().typeStackPushBack(sym);
    }

    template<typename Formatter>
    void EnterTypeNameVisitor<Formatter>::visit(ptr<const Symbol<Type<Derived> > > sym)
    {
      attributeManager.getInheritedAttribute().typeStackPushBack(sym);
    }

    template<typename Formatter>
    void BeforeTypeNameVisitor<Formatter>::visit(ptr<const Symbol<Type<TypeBase> > > sym)
    {
      if (!sym) {
        // This is an empty return type.
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "void";
      }
    }

    template<typename Formatter>
    void BeforeTypeNameVisitor<Formatter>::visit(ptr<const Symbol<Type<Derived> > > sym)
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

    template<typename Formatter>
    void LeaveTypeNameVisitor<Formatter>::visit(ptr<const Symbol<Type<Pointer> > > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << " *";
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    template<typename Formatter>
    void LeaveTypeNameVisitor<Formatter>::visit(ptr<const Symbol<Type<FunctionType> > > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      if (sym->isVararg()) {
        out << ", ...";
      }
      out << ")";
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    template<typename Formatter>
    void LeaveTypeNameVisitor<Formatter>::visit(ptr<const Symbol<Type<Tuple> > > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << ")";
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    template<typename Formatter>
    void LeaveTypeNameVisitor<Formatter>::visit(ptr<const Symbol<Type<Derived> > > sym)
    {
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<std::int8_t> > > sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<std::uint8_t> > > sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<std::int16_t> > > sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<std::uint16_t> > > sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<std::int32_t> > > sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<std::uint32_t> > > sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<std::int64_t> > > sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<std::uint64_t> > > sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<float> > > sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<double> > > sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<std::string> > > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << '"' << boost::replace_all_copy(sym->value(),
                                            "\n", "\\n") << '"';
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<const Symbol<Constant<Address> > > sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<const Symbol<Module> > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "mdef " << sym->name() << " {"
        << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<const Symbol<Function> > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      // Just declarations.
      out << Indent<Formatter>(ind) << "fdecl " << sym->name() << ' ';
      TypeNameFlow<Formatter> typePrinter(out);
      sym->type()->accept(typePrinter);
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<const Symbol<Variable> > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "vdecl " << sym->name() << " "; 
      TypeNameFlow<Formatter> typePrinter(out);
      sym->type()->accept(typePrinter);
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<const Symbol<GlobalVariable> > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "gvdecl " << sym->name() << " "; 
      TypeNameFlow<Formatter> typePrinter(out);
      sym->type()->accept(typePrinter);
      if (sym->initializer()) {
        out << ' ';
        printImpl<detail::CompactFormatter>(out, sym->initializer());
      }
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<const Symbol<Type<TypeBase> > > sym)
    {
      // This is a type that doesn't need a declaration.  So tell the
      // printer not to print a newline after it.
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<const Symbol<Type<Tuple> > > sym)
    {
      // Stream &out = attributeManager.getInheritedAttribute().out();
      // int ind = attributeManager.getInheritedAttribute().indent();

      // out << Indent<Formatter>(ind) << "tdecl " << sym->name();
      // out << " (\n";
      // for (Symbol<Type<Tuple> >::const_iterator p =
      //        sym->begin(), pend = sym->end();
      //      p != pend;
      //      /* NULL */) {
      //   out << indent(ind+Formatter::indentFactor()) << (*p)->name();
      //   if (++p != pend) {
      //     out << ',';
      //   }
      //   out << '\n';
      // }
      // out << Indent<Formatter>(ind) << ")";
    }

    /// Print the final newline after each symbol declaration.
    template<typename Formatter>
    void LeaveDeclSymbolVisitor<Formatter>::visit(ptr<const Symbol<Base> >)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveDeclSymbolVisitor<Formatter>::visit(ptr<const Symbol<Type<TypeBase> > >)
    {
      // Don't do anything since we didn't print anything for this
      // type.
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    /// Print the final newline after tuple types.
    template<typename Formatter>
    void LeaveDeclSymbolVisitor<Formatter>::visit(ptr<const Symbol<Type<Tuple> > >)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void EnterDefSymbolVisitor<Formatter>::visit(ptr<const Symbol<Module> > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterDefSymbolVisitor<Formatter>::visit(ptr<const Symbol<Function> > sym)
    {
      if (!sym->statementEmpty()) {
        Stream &out = attributeManager.getInheritedAttribute().out();
        int ind = attributeManager.getInheritedAttribute().indent();

        out << Indent<Formatter>(ind) << "fdef " << sym->name() << " {"
          << Newline<Formatter>();
        attributeManager.setInheritedAttribute(
          InheritedAttribute(ind + Formatter::indentFactor(), out));
      }
    }

    template<typename Formatter>
    void EnterDefSymbolVisitor<Formatter>::visit(ptr<const Symbol<Variable> > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "vdecl " << sym->name() << " ";
      TypeNameFlow<Formatter> typePrinter(out);
      sym->type()->accept(typePrinter);
    }

    template<typename Formatter>
    void EnterDefSymbolVisitor<Formatter>::visit(ptr<const Symbol<GlobalVariable> > sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "gvdecl " << sym->name() << " ";
      TypeNameFlow<Formatter> typePrinter(out);
      sym->type()->accept(typePrinter);
      if (sym->initializer()) {
        out << ' ';
        printImpl<detail::CompactFormatter>(out, sym->initializer());
      }
    }

    template<typename Formatter>
    void LeaveDefSymbolVisitor<Formatter>::visit(ptr<const Symbol<Variable> > sym)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveDefSymbolVisitor<Formatter>::visit(ptr<const Symbol<GlobalVariable> > sym)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveDefSymbolVisitor<Formatter>::visit(ptr<const Symbol<Module> > sym) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "}" << Newline<Formatter>();
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveDefSymbolVisitor<Formatter>::visit(ptr<const Symbol<Function> > sym) {
      if (!sym->statementEmpty()) {
        Stream &out = attributeManager.getInheritedAttribute().out();
        int ind = attributeManager.getInheritedAttribute().indent();

        if (   !attributeManager.setLastSynthesizedAttribute()
               || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
          out << Newline<Formatter>();
        }
        out << Indent<Formatter>(ind) << '}' << Newline<Formatter>();
        attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
      }
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<Block> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "{" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<IfThen> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "ifThen" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<IfElse> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "ifElse" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<While> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "while" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<DoWhile> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "doWhile" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<Switch> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "switch" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<Case> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "case";
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<CaseBlock> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "caseblock";
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<Before> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "before ";
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<After> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "after ";
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<Goto> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "goto ";
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<Return> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "return" << Newline<Formatter>();
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<Phi> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "phi" << Newline<Formatter>();
      out << Indent<Formatter>(ind + Formatter::indentFactor())
          << stmt->target()->name() << Newline<Formatter>();

      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<Store> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "store" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<Call> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "call" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<const Statement<Allocate> > stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "allocate ";
      TypeNameFlow<Formatter> typePrinter(out);
      stmt->type()->accept(typePrinter);
      out << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void LeaveStatementVisitor<Formatter>::visit(ptr<const Statement<Base> > stmt)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveStatementVisitor<Formatter>::visit(ptr<const Statement<Block> > stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << Newline<Formatter>();
      }
      out << Indent<Formatter>(ind) << '}' << Newline<Formatter>();
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveStatementVisitor<Formatter>::visit(ptr<const Statement<Return> > stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();

      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Add> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '+' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Subtract> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '-' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Multiply> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '*' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Divide> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '/' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Modulus> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '%' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Negate> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "neg" << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<LogicalAnd> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "&&" << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<LogicalOr> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "||" << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<LogicalNot> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '!' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<BitwiseAnd> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '&' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<BitwiseOr> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '|' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<BitwiseComplement> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '~' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<LessThan> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '<' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<LessThanOrEqual> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "<=" << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Equal> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "==" << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<NotEqual> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "!=" << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<GreaterThan> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '>' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<GreaterThanOrEqual> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << ">=" << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<TuplePointer> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '&' << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Variable> > > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "vref " << expr->getSymbol()->name() << Operator<Formatter>(expr);
    }

    template<typename Formatter>
    template<typename ValueType>
    void EnterExpressionVisitor<Formatter>::visitConstant(boost::shared_ptr<const Expression<Reference<Constant<ValueType> > > > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "cref ";
      TypeNameFlow<Formatter> typePrinter(out);
      expr->getSymbol()->type()->accept(typePrinter);
      out << ' ';
      ConstantValueFlow<Formatter> valuePrinter(out);
      expr->getSymbol()->accept(valuePrinter);
      out << Operator<Formatter>(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<Base> > > > expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<std::int8_t> > > > expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<std::uint8_t> > > > expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<std::int16_t> > > > expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<std::uint16_t> > > > expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<std::int32_t> > > > expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<std::uint32_t> > > > expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<std::int64_t> > > > expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<std::uint64_t> > > > expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<float> > > > expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<double> > > > expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Constant<std::string> > > > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "cref ";
      TypeNameFlow<Formatter> typePrinter(out);
      expr->getSymbol()->type()->accept(typePrinter);
      out << ' ';
      ConstantValueFlow<Formatter> valuePrinter(out);
      expr->getSymbol()->accept(valuePrinter);
      out << Operator<Formatter>(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Reference<Function> > > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "fref " << expr->getSymbol()->name() << Operator<Formatter>(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<const Expression<Load> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "load" << Operator<Formatter>(expr) << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void LeaveExpressionVisitor<Formatter>::visit(ptr<const Expression<Base> > expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      if (   !attributeManager.setLastSynthesizedAttribute()
          || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveExpressionVisitor<Formatter>::visit(ptr<const InnerExpression> expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      if (   !attributeManager.setLastSynthesizedAttribute()
          || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void PrintDefSymbolFlow<Formatter>::visit(ptr<const Symbol<Module>> sym) {
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
          ptr<const Expression<Base> > expression;

        public:
          Operator(ptr<const Expression<Base> > expr)
              : expression(expr) {}
          Stream &operator()(Stream &out) const {
            out << ' ';
            TypeNameFlow<DefaultFormatter> typePrinter(out);
            expression->type()->accept(typePrinter);
            out << ' ';
            return out;
          }
        };
      };
    }
  }

  void print(Printer::Stream &out, ptr<const Node<Base>> node)
  {
    Printer::printImpl(out, node);
  }

  void printWithTypes(Printer::Stream &out, ptr<const Node<Base>> node)
  {
    Printer::printImpl<Printer::detail::TypeFormatter>(out, node);
  }
}
