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
      void printImpl(Stream &out, ptr<Node<Base> >::const_type node);
    }

    typedef std::vector<ptr<Symbol<Type<TypeBase> > >::const_type> TypeList;

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
    template<typename Formatter>
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

      void visit(ptr<Symbol<Type<TypeBase> > >::const_type);
      void visit(ptr<Symbol<Type<Derived> > >::const_type);
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
        if (ptr<Symbol<Type<FunctionType> > >::const_type function =
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

      void visit(ptr<Symbol<Type<Pointer> > >::const_type);
      void visit(ptr<Symbol<Type<FunctionType> > >::const_type);
      void visit(ptr<Symbol<Type<Tuple> > >::const_type);
      void visit(ptr<Symbol<Type<Derived> > >::const_type);
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
          : BaseType(TypeNameInheritedAttribute(out, typeStack)) {}
    };
 
    /// Define the inherited attibute.
    class InheritedAttribute {
    private:
      /// The current indent level.
      int ind;
      /// The stream to dump to.
      Stream *ot;

    public:
      InheritedAttribute(void) : ind(0), ot(0) {}
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

      void visit(ptr<Symbol<Constant<std::int8_t> > >::const_type);
      void visit(ptr<Symbol<Constant<std::uint8_t> > >::const_type);
      void visit(ptr<Symbol<Constant<std::int16_t> > >::const_type);
      void visit(ptr<Symbol<Constant<std::uint16_t> > >::const_type);
      void visit(ptr<Symbol<Constant<std::int32_t> > >::const_type);
      void visit(ptr<Symbol<Constant<std::uint32_t> > >::const_type);
      void visit(ptr<Symbol<Constant<std::int64_t> > >::const_type);
      void visit(ptr<Symbol<Constant<std::uint64_t> > >::const_type);
      void visit(ptr<Symbol<Constant<float> > >::const_type);
      void visit(ptr<Symbol<Constant<double> > >::const_type);
      void visit(ptr<Symbol<Constant<std::string> > >::const_type);
      void visit(ptr<Symbol<Constant<Address> > >::const_type);
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
          : BaseType(InheritedAttribute(0, out)) {}
    };
 
    /// Entering each symbol
    template<typename Formatter>
    class EnterDeclSymbolVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterDeclSymbolVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<Symbol<Module> >::const_type sym);
      void visit(ptr<Symbol<Function> >::const_type sym);
      void visit(ptr<Symbol<Variable> >::const_type sym);
      void visit(ptr<Symbol<GlobalVariable> >::const_type sym);
      void visit(ptr<Symbol<Type<TypeBase> > >::const_type sym);
      void visit(ptr<Symbol<Type<Tuple> > >::const_type sym);
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

      void visit(ptr<Symbol<Module> >::const_type sym);
      void visit(ptr<Symbol<Function> >::const_type sym);
      void visit(ptr<Symbol<Variable> >::const_type sym);
      void visit(ptr<Symbol<GlobalVariable> >::const_type sym);
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
      void visit(ptr<Symbol<Base> >::const_type);

      /// Do nothing for most types.
      void visit(ptr<Symbol<Type<TypeBase> >  >::const_type);

      /// Print the final newline after tuple types.
      void visit(ptr<Symbol<Type<Tuple> > >::const_type);
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
      void visit(ptr<Symbol<Variable> >::const_type sym);
      void visit(ptr<Symbol<GlobalVariable> >::const_type sym);
      void visit(ptr<Symbol<Module> >::const_type sym);
      void visit(ptr<Symbol<Function> >::const_type sym);
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
      void visit(ptr<Statement<Phi> >::const_type stmt);
      void visit(ptr<Statement<Assignment> >::const_type stmt);
      void visit(ptr<Statement<Call> >::const_type stmt);
      void visit(ptr<Statement<Allocate> >::const_type stmt);
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

      void visit(ptr<Statement<Base> >::const_type stmt);
      void visit(ptr<Statement<Block> >::const_type stmt);
      void visit(ptr<Statement<Return> >::const_type stmt);
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
      void visit(ptr<Expression<Reference<Constant<std::string> > > >::const_type expr);
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

      void visit(ptr<Expression<Base> >::const_type expr);
      void visit(ptr<InnerExpression>::const_type expr);
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
          : BaseType(InheritedAttribute(0, out)) {}

      PrintExpressionFlow(FlowAttributeManagerType &)
          : BaseType(InheritedAttribute()) {}        

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
      PrintFlow(Stream &out) : BaseType(InheritedAttribute(0, out)) {
        this->expression().setParentFlow(this);
      }

      PrintFlow(FlowAttributeManagerType &)
          : BaseType(InheritedAttribute()) {
        this->expression().setParentFlow(this);
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

      // We need to reverse the order in which we visit the
      // allocate operands.
      void visit(ptr<Statement<Allocate> >::const_type stmt) {
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
          : BaseType(InheritedAttribute(0, out)) {}
      
      // We just want the function declaration at this point so don't
      // visit children.
      void visit(ptr<Symbol<Function> >::const_type sym) {
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
          : BaseType(InheritedAttribute(i, out)) {
        this->statement().setParentFlow(this);
      }

      /// We only want to visit functions here since we already
      /// declared module-level types and variables.
      void visit(ptr<Symbol<Module> >::const_type sym);
    };

    namespace {
      template<typename Formatter = Printer::detail::DefaultFormatter>
      void printImpl(Printer::Stream &out, ptr<Node<Base> >::const_type node)
      {
        if (ptr<Symbol<Module> >::const_type s =
            boost::dynamic_pointer_cast<const Symbol<Module> >(node)) {
          ptr<ConstSymbolVisitor>::type declflow(new Printer::PrintDeclSymbolFlow<Formatter>(out));
          s->accept(*declflow);
          ptr<ConstSymbolVisitor>::type defflow(new Printer::PrintDefSymbolFlow<Formatter>(out));
          s->accept(*defflow);
        }
        else if (ptr<Symbol<Type<TypeBase> > >::const_type t =
                 boost::dynamic_pointer_cast<const Symbol<Type<TypeBase> > >(node)) {
          ptr<ConstSymbolVisitor>::type flow(new Printer::TypeNameFlow<Formatter>(out));
          t->accept(*flow);
        }
        else if (ptr<Symbol<Base> >::const_type s =
                 boost::dynamic_pointer_cast<const Symbol<Base> >(node)) {
          ptr<ConstSymbolVisitor>::type defflow(new Printer::PrintDefSymbolFlow<Formatter>(out));
          s->accept(*defflow);
        }
        else if (ptr<Statement<Base> >::const_type s =
                 boost::dynamic_pointer_cast<const Statement<Base> >(node)) {
          ptr<ConstStatementVisitor>::type flow(new Printer::PrintFlow<Formatter>(out));
          s->accept(*flow);
        }
        else if (ptr<Expression<Base> >::const_type e =
                 boost::dynamic_pointer_cast<const Expression<Base> >(node)) {
          ptr<ConstExpressionVisitor>::type flow(new Printer::PrintExpressionFlow<Formatter>(out));
          e->accept(*flow);
        }
      }
    }

    template<typename Formatter>
    void EnterTypeNameVisitor<Formatter>::visit(ptr<Symbol<Type<Integral> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << "int" << sym->integerBitSize();
    }

    template<typename Formatter>
    void EnterTypeNameVisitor<Formatter>::visit(ptr<Symbol<Type<Floating> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << "float" << sym->integerBitSize();
    }

    template<typename Formatter>
    void EnterTypeNameVisitor<Formatter>::visit(ptr<Symbol<Type<Tuple> > >::const_type sym)
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
    void EnterTypeNameVisitor<Formatter>::visit(ptr<Symbol<Type<Derived> > >::const_type sym)
    {
      attributeManager.getInheritedAttribute().typeStackPushBack(sym);
    }

    template<typename Formatter>
    void BeforeTypeNameVisitor<Formatter>::visit(ptr<Symbol<Type<TypeBase> > >::const_type sym)
    {
      if (!sym) {
        // This is an empty return type.
        Stream &out = attributeManager.getInheritedAttribute().out();
        out << "void";
      }
    }

    template<typename Formatter>
    void BeforeTypeNameVisitor<Formatter>::visit(ptr<Symbol<Type<Derived> > >::const_type sym)
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
    void LeaveTypeNameVisitor<Formatter>::visit(ptr<Symbol<Type<Pointer> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << " *";
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    template<typename Formatter>
    void LeaveTypeNameVisitor<Formatter>::visit(ptr<Symbol<Type<FunctionType> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      if (sym->isVararg()) {
        out << ", ...";
      }
      out << ")";
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    template<typename Formatter>
    void LeaveTypeNameVisitor<Formatter>::visit(ptr<Symbol<Type<Tuple> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << ")";
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    template<typename Formatter>
    void LeaveTypeNameVisitor<Formatter>::visit(ptr<Symbol<Type<Derived> > >::const_type sym)
    {
      attributeManager.getInheritedAttribute().typeStackPopBack();
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<std::int8_t> > >::const_type sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<std::uint8_t> > >::const_type sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<std::int16_t> > >::const_type sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<std::uint16_t> > >::const_type sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<std::int32_t> > >::const_type sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<std::uint32_t> > >::const_type sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<std::int64_t> > >::const_type sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<std::uint64_t> > >::const_type sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<float> > >::const_type sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<double> > >::const_type sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<std::string> > >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      out << '"' << boost::replace_all_copy(sym->value(),
                                            "\n", "\\n") << '"';
    }

    template<typename Formatter>
    void EnterConstantValueVisitor<Formatter>::visit(ptr<Symbol<Constant<Address> > >::const_type sym)
    {
      this->visitPrimitiveConstant(sym);
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<Symbol<Module> >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "mdef " << sym->name() << " {"
        << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<Symbol<Function> >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      // Just declarations.
      out << Indent<Formatter>(ind) << "fdecl " << sym->name() << ' ';
      TypeNameFlow<Formatter> typePrinter(out);
      sym->type()->accept(typePrinter);
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<Symbol<Variable> >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "vdecl " << sym->name() << " "; 
      TypeNameFlow<Formatter> typePrinter(out);
      sym->type()->accept(typePrinter);
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<Symbol<GlobalVariable> >::const_type sym)
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
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<Symbol<Type<TypeBase> > >::const_type sym)
    {
      // This is a type that doesn't need a declaration.  So tell the
      // printer not to print a newline after it.
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void EnterDeclSymbolVisitor<Formatter>::visit(ptr<Symbol<Type<Tuple> > >::const_type sym)
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
    void LeaveDeclSymbolVisitor<Formatter>::visit(ptr<Symbol<Base> >::const_type)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveDeclSymbolVisitor<Formatter>::visit(ptr<Symbol<Type<TypeBase> > >::const_type)
    {
      // Don't do anything since we didn't print anything for this
      // type.
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    /// Print the final newline after tuple types.
    template<typename Formatter>
    void LeaveDeclSymbolVisitor<Formatter>::visit(ptr<Symbol<Type<Tuple> > >::const_type)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void EnterDefSymbolVisitor<Formatter>::visit(ptr<Symbol<Module> >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterDefSymbolVisitor<Formatter>::visit(ptr<Symbol<Function> >::const_type sym)
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
    void EnterDefSymbolVisitor<Formatter>::visit(ptr<Symbol<Variable> >::const_type sym)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "vdecl " << sym->name() << " ";
      TypeNameFlow<Formatter> typePrinter(out);
      sym->type()->accept(typePrinter);
    }

    template<typename Formatter>
    void EnterDefSymbolVisitor<Formatter>::visit(ptr<Symbol<GlobalVariable> >::const_type sym)
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
    void LeaveDefSymbolVisitor<Formatter>::visit(ptr<Symbol<Variable> >::const_type sym)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveDefSymbolVisitor<Formatter>::visit(ptr<Symbol<GlobalVariable> >::const_type sym)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveDefSymbolVisitor<Formatter>::visit(ptr<Symbol<Module> >::const_type sym) {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "}" << Newline<Formatter>();
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveDefSymbolVisitor<Formatter>::visit(ptr<Symbol<Function> >::const_type sym) {
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
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<Block> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "{" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<IfThen> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "ifThen" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<IfElse> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "ifElse" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<While> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "while" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<DoWhile> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "doWhile" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<Switch> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "switch" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<Case> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "case";
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<CaseBlock> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "caseblock";
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<Before> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "before ";
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<After> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "after ";
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<Goto> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "goto ";
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<Return> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "return" << Newline<Formatter>();
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<Phi> >::const_type stmt)
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
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<Assignment> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "assign" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<Call> >::const_type stmt)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind) << "call" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterStatementVisitor<Formatter>::visit(ptr<Statement<Allocate> >::const_type stmt)
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
    void LeaveStatementVisitor<Formatter>::visit(ptr<Statement<Base> >::const_type stmt)
    {
      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        attributeManager.getInheritedAttribute().out()
          << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveStatementVisitor<Formatter>::visit(ptr<Statement<Block> >::const_type stmt) {
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
    void LeaveStatementVisitor<Formatter>::visit(ptr<Statement<Return> >::const_type stmt) {
      Stream &out = attributeManager.getInheritedAttribute().out();

      if (   !attributeManager.setLastSynthesizedAttribute()
             || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Add> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '+' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Subtract> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '-' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Multiply> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '*' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Divide> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '/' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Modulus> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '%' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Negate> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "neg" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<LogicalAnd> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "&&" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<LogicalOr> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "||" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<LogicalNot> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '!' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<BitwiseAnd> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '&' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<BitwiseOr> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '|' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<BitwiseComplement> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '~' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<LessThan> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '<' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<LessThanOrEqual> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "<=" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Equal> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "==" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<NotEqual> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "!=" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<GreaterThan> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '>' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<GreaterThanOrEqual> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << ">=" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<TuplePointer> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << '&' << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Variable> > >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "vref " << expr->getSymbol()->name();
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
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<Base> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<std::int8_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<std::uint8_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<std::int16_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<std::uint16_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<std::int32_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<std::uint32_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<std::int64_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<std::uint64_t> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<float> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<double> > > >::const_type expr)
    {
      visitConstant(expr);
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Constant<std::string> > > >::const_type expr)
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
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Function> > >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "fref " << expr->getSymbol()->name();
    }

    template<typename Formatter>
    void EnterExpressionVisitor<Formatter>::visit(ptr<Expression<Reference<Tuple> > >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      int ind = attributeManager.getInheritedAttribute().indent();

      out << Indent<Formatter>(ind);
      out << "tref" << Newline<Formatter>();
      attributeManager.setInheritedAttribute(
        InheritedAttribute(ind + Formatter::indentFactor(), out));
    }

    template<typename Formatter>
    void LeaveExpressionVisitor<Formatter>::visit(ptr<Expression<Base> >::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      if (   !attributeManager.setLastSynthesizedAttribute()
          || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void LeaveExpressionVisitor<Formatter>::visit(ptr<InnerExpression>::const_type expr)
    {
      Stream &out = attributeManager.getInheritedAttribute().out();
      if (   !attributeManager.setLastSynthesizedAttribute()
          || !attributeManager.getLastSynthesizedAttribute().justLeft()) {
        out << Newline<Formatter>();
      }
      attributeManager.setSynthesizedAttribute(SynthesizedAttribute(true));
    }

    template<typename Formatter>
    void PrintDefSymbolFlow<Formatter>::visit(ptr<Symbol<Module>>::const_type sym) {
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

  void print(Printer::Stream &out, ptr<Node<Base>>::const_type node)
  {
    Printer::printImpl(out, node);
  }
}
