#ifndef mirv_Filter_Snapshot_Codegen_LLVM_hpp
#define mirv_Filter_Snapshot_Codegen_LLVM_hpp

#include <mirv/Core/Filter/AttributeFlow.hpp>
#include <mirv/Core/Filter/ForwardFlow.hpp>
#include <mirv/Core/Filter/ExpressionFlow.hpp>
#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Filter/FlowAction.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/IR/Node.hpp>

namespace mirv {
  /// This is a filter to translate from MIRV IR to LLVM IR.
  class LLVMCodegenFilter
      : public Filter<Node<Base> > {
  private:
    class InheritedAttribute;

    class SynthesizedAttribute {
    private:
      llvm::Context Context;
      ptr<llvm::IRBuilder>::type Builder;
      llvm::Module *TheModule;
      llvm::Function *TheFunction;
      llvm::Value *value;n
      bool ReturnValue;

    public:
      SynthesizedAttribute(llvm::Value *v = 0, bool isReturn = false) 
          : Context(llvm::getGlobalContext()),
              Builder(new llvm::IRBuilder(Context)),
              TheModule(0),
              TheFunction(0),
              value(v),
              ReturnValue(isReturn) {}

      SynthesizedAttribute(const InheritedAttribute &inherited);

      llvm::Value *getValue(void) const {
        checkInvariant(value, "No value");
        return value;
      }
      bool hasReturnValue(void) const {
        return ReturnValue;
      }
    };

    class InheritedAttribute {
    private:
      llvm::Context &Context;
      ptr<llvm::IRBuilder>::type Builder;
      llvm::Module *TheModule;
      llvm::Function *TheFunction;
      llvm::Value *TheValue;
      bool ReturnValue;
      bool GenerateAddress;

      class TypeCreator : public SymbolVisitor {
      private:
        llvm::Context &Context;
        llvm::Type *TheType;

      public:
        TypeCreator(llvm::Context &context) 
            : Context(context) {}

        virtual void visit(ptr<Symbol<Type<Integral> > >::type);
        virtual void visit(ptr<Symbol<Type<Floating> > >::type);
        virtual void visit(ptr<Symbol<Type<Array> > >::type);
        virtual void visit(ptr<Symbol<Type<Pointer> > >::type);
        virtual void visit(ptr<Symbol<Type<FunctionType> > >::type);
        virtual void visit(ptr<Symbol<Type<StructType> > >::type);

        llvm::Type *type(void) const {
          return TheType;
        }
      };

      llvm::Type *getType(ptr<Symbol<Type<TypeBase> > >::type) const;

    public:
      InheritedAttribute(bool address = false) 
          : Context(llvm::getGlobalContext()),
              Builder(new llvm::IRBuilder(Context)),
              TheModule(0),
              TheFunction(0),
              TheValue(0),
              ReturnValue(false),
              GenerateAddress(address) {}

      InheritedAttribute(const InheritedAttribute &other) 
          : Context(other.Context),
              Builder(other.Builder),
              TheModule(other.TheModule),
              TheFunction(other.TheFunction),
              TheValue(other.TheValue),
              ReturnValue(other.ReturnValue),
              // Do not inherit this property.
              GenerateAddress(false) {}

      InheritedAttribute(const SynthesizedAttribute &synthesized) 
          : Context(synthesized.Context),
              Builder(synthesized.Builder),
              TheModule(synthesized.TheModule),
              TheFunction(synthesized.TheFunction),
              TheValue(synthesized.value),
              ReturnValue(synthesized.ReturnValue),
              GenerateAddress(false) {}

      ptr<llvm::IRBuilder>::type builder(void) {
        return Builder;
      }

      llvm::Value *getValue(void) const {
        checkInvariant(TheValue, "No value");
        return TheValue;
      }

      void setValue(llvm::Value *v, bool isReturn = false) {
        checkInvariant(v, "No value");
        TheValue = v;
        if (isReturn) {
          ReturnValue = true;
        }
      }

      bool hasReturnValue(void) const {
        return ReturnValue;
      }

      bool generateAddress(void) const {
        return GenerateAddress;
      }

      void createModule(const std::string &name) {
        checkInvariant(Module == 0, "Module already exists");
        TheModule = new llvm::Module(name, getGlobalContext());
      }

      void createFunction(const std::string &name,
                          ptr<Symbol<Type<TypeBase> > > type);

      void createVariable(const std::string &name,
                          ptr<Symbol<Type<TypeBase> > > type);

      void createBlock(const std::string &name) {
        checkInvariant(TheFunction, "No function for block");
        llvm::BasicBlock *block = llvm::BasicBlock::Create(Context,
                                                           name,
                                                           TheFunction);
        builder()->setInsertPt(block);
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
      void visit(ptr<Symbol<Base> >::type sym) {}
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
      void visit(ptr<Symbol<Variable> >::type sym) {}
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
      void visit(ptr<Statement<Before> >::type stmt);
      void visit(ptr<Statement<After> >::type stmt);
      void visit(ptr<Statement<Goto> >::type stmt);
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

      void visit(ptr<Statement<Block> >::type stmt);
      void visit(ptr<Statement<Before> >::type stmt);
      void visit(ptr<Statement<After> >::type stmt);
      void visit(ptr<Statement<Goto> >::type stmt);
    };

    class LeaveStatementAction : public VisitAction<LeaveStatementVisitor> {
    public:
      LeaveStatementAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveStatementVisitor>(attributeManager) {}
    };

    /// Leaveing each expression
    class LeaveExpressionVisitor : public ExpressionVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveExpressionVisitor(FlowAttributeManagerType &am)
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

    class LeaveExpressionAction : public VisitAction<LeaveExpressionVisitor> {
    public:
      LeaveExpressionAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveExpressionVisitor>(attributeManager) {}
    };

    /// This is the flow for translating expressions.
    class LLVMCodegenExpressionFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardExpressionFlowGenerator,
      NullAction,
      LeaveExpressionAction,
      NullAction,
      NullAction,
      AttributeFlowBetweenAction<NullAction, FlowAttributeManagerType>
      > {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardExpressionFlowGenerator,
      NullAction,
      LeaveExpressionAction,
      NullAction,
      NullAction,
      AttributeFlowBetweenAction<NullAction, FlowAttributeManagerType>
      > BaseType;

    public:
      LLVMCodegenExpressionFlow()
          : BaseType(InheritedAttribute()) {}

      LLVMCodegenExpressionFlow(FlowAttributeManagerType &)
          : BaseType(InheritedAttribute()) {}        
    };

    /// This is the flow for translating statements.
    class LLVMCodegenFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardFlowGenerator,
      EnterStatementAction,
      LeaveStatementAction,
      // Transfer synthesized to inherited before statements.
      AttributeFlowBetweenAction<NullAction, FlowAttributeManagerType>,
      NullAction,
      NullAction,
      NullJoinAction,
      // Transfer synthesized to inherited before expressions.
      AttributeFlowBetweenAction<NullAction, FlowAttributeManagerType>,
      FlowAction<LLVMCodegenFlow, LLVMCodegenExpressionFlow>,
      NullAction> {
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
        FlowAction<LLVMCodegenFlow, LLVMCodegenExpressionFlow>,
	NullAction> BaseType;

    public:
      LLVMCodegenFlow()
          : BaseType(InheritedAttribute()) {}
    };

  public:
    LLVMCodegenFilter(void) : Filter<Node<Base> >() {}

    /// Translate an IR tree.
    void operator()(ptr<Node<Base> >::type node);
  };
}

#endif
