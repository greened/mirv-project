#ifndef mirv_Filter_Snapshot_Codegen_LLVM_hpp
#define mirv_Filter_Snapshot_Codegen_LLVM_hpp

#include <mirv/Core/Filter/AttributeFlow.hpp>
#include <mirv/Core/Filter/SymbolVisitor.hpp>
#include <mirv/Core/Filter/ForwardControlFlow.hpp>
#include <mirv/Core/Filter/ValueFlow.hpp>
#include <mirv/Core/Filter/SymbolFlow.hpp>
#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Filter/FlowAction.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/Filter/SymbolFlow.hpp>
#include <mirv/Library/Map.hpp>

#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

#include <utility>

namespace mirv {
  /// This is a filter to translate from MIRV IR to LLVM IR.
  class LLVMCodegenFilter final : public Filter {
  public:
    class InheritedAttribute;
    class SynthesizedAttribute;

    /// This is the base of codegen inherited and synthesized
    /// attributes.  It contains all of the necessary context for
    /// creating IR constructs and references for holding codegen
    /// results for parent nodes to examine.
    class FlowAttribute {
    private:
      llvm::LLVMContext &Context;
      ptr<llvm::IRBuilder<> > Builder;
      llvm::Module *TheModule;
      llvm::Function *TheFunction;
      llvm::Function *ReferencedFunction;
      llvm::BasicBlock *TheBlock;
      llvm::Value *TheValue;
      bool ReturnValue;

      typedef Map<std::string, llvm::Value *> VariableMap;
      ptr<VariableMap> ModuleMap;
      ptr<VariableMap> FunctionMap;

      class TypeCreator : public SymbolVisitor {
      private:
        llvm::LLVMContext &Context;
        llvm::Type *TheType;

      public:
        TypeCreator(llvm::LLVMContext &context)
            : Context(context) {}

        virtual void visit(const IntegerType &);
        virtual void visit(const FloatingPointType &);
        virtual void visit(const TupleType &);
        virtual void visit(const PointerType &);
        virtual void visit(const FunctionType &);

        llvm::Type *type(void) const {
          return TheType;
        }
      };

    protected:
      void setValueUnchecked(llvm::Value *v) {
        TheValue = v;
      }

    public:
      FlowAttribute(llvm::LLVMContext &C)
          : Context(C),
              Builder(new llvm::IRBuilder<>(Context)),
              TheModule(nullptr),
              TheFunction(nullptr),
              TheBlock(nullptr),
              TheValue(nullptr),
              ReturnValue(false),
              ModuleMap(new VariableMap),
              FunctionMap(new VariableMap) {}

      FlowAttribute(const FlowAttribute &other)
          : Context(other.Context),
              Builder(other.Builder),
              TheModule(other.TheModule),
              TheFunction(other.TheFunction),
              TheBlock(other.TheBlock),
              TheValue(other.TheValue),
              ReturnValue(other.ReturnValue),
              ModuleMap(other.ModuleMap),
              FunctionMap(other.FunctionMap) {}

      FlowAttribute(const FlowAttribute &&other)
          : Context(other.Context),
              Builder(other.Builder),
              TheModule(other.TheModule),
              TheFunction(other.TheFunction),
              TheBlock(other.TheBlock),
              TheValue(other.TheValue),
              ReturnValue(other.ReturnValue),
              ModuleMap(other.ModuleMap),
              FunctionMap(other.FunctionMap) {}

      void clearFunctionMap(void) {
        FunctionMap->clear();
      }

      ptr<llvm::IRBuilder<> > builder(void) const {
        return Builder;
      }

      llvm::Value *getValue(void) const {
        checkInvariant(TheValue, "No value");
        return TheValue;
      }

      void setValue(llvm::Value *v) {
        checkInvariant(v != 0, "Null value");
        TheValue = v;
      }

      bool hasReturnValue(void) const {
        return ReturnValue;
      }

      bool hasBlock(void) const {
        return TheBlock != 0;
      }

      llvm::BasicBlock *getBlock(void) const {
        checkInvariant(TheBlock, "No block");
        return TheBlock;
      }

      llvm::Type *
      getType(ptr<const Type>) const;

      void createModule(const std::string &name) {
        checkInvariant(TheModule == 0, "Module already exists");
        TheModule = new llvm::Module(name, Context);
      }

      llvm::Module *getModule(void) const {
        checkInvariant(TheModule, "Null module");
        return TheModule;
      }

      llvm::Function *getFunction(void) const {
        checkInvariant(TheFunction, "Null function");
        return TheFunction;
      }

      void createFunction(const std::string &name,
                          ptr<const Type> type);

      void setReferencedFunction(llvm::Function *function) {
        ReferencedFunction = function;
      }

      llvm::Function *referencedFunction(void) const {
        return ReferencedFunction;
      }

      void createVariable(const std::string &name,
                          ptr<const Type> type);

      void createAlloca(const std::string &name,
                        ptr<const Type> type);

      void createGlobalVariable(const GlobalVariable &sym,
                                const InheritedAttribute &inh,
                                const SynthesizedAttribute &syn);

      template<typename ValueType>
      void createIntegerConstant(ptr<const Type> type,
                                 ValueType value,
                                 bool isSigned) {
        llvm::Type *llvmType = getType(type);
        llvm::Value *constant = llvm::ConstantInt::get(llvmType, value, isSigned);
        setValue(constant);
      }

      template<typename ValueType>
      void createFloatingPointConstant(ptr<const Type> type,
                                       ValueType value) {
        llvm::Type *llvmType = getType(type);
        llvm::Value *constant = llvm::ConstantFP::get(llvmType, value);
        setValue(constant);
      }

      llvm::Value *getGlobalVariable(const std::string &name);

      llvm::BasicBlock *createBlock(const std::string &name) {
        checkInvariant(TheFunction, "No function for block");
        llvm::BasicBlock *block = llvm::BasicBlock::Create(Context,
                                                           name,
                                                           TheFunction);
        builder()->SetInsertPoint(block);
        TheBlock = block;
        return block;
      }
    };

    /// This is the inherited attribute for code generation.  It
    /// carries the necessary context and utility functions to create
    /// IR constructs.
    class InheritedAttribute
        : public FlowAttribute {
    public:
      InheritedAttribute(llvm::LLVMContext &C) : FlowAttribute(C) {};

      InheritedAttribute(const InheritedAttribute &inherited)
          : FlowAttribute(inherited) {}

      InheritedAttribute(const InheritedAttribute &&inherited)
          : FlowAttribute(inherited) {}

      InheritedAttribute(const SynthesizedAttribute &synthesized)
          : FlowAttribute(synthesized) {
        // Do not inherited this property.
        setValueUnchecked(0);
      }

      InheritedAttribute &operator=(const InheritedAttribute &other) {
        InheritedAttribute temp(other);
        std::swap(temp, *this);
        return *this;
      }

      InheritedAttribute &operator=(const InheritedAttribute &&other) {
        InheritedAttribute temp(other);
        std::swap(temp, *this);
        return *this;
      }

      InheritedAttribute &operator=(const SynthesizedAttribute &synthesized) {
        InheritedAttribute temp(synthesized);
        std::swap(temp, *this);
        return *this;
      }
    };

    /// This is the synthesized attribute for the code generator.  It
    /// carrries result Values, Symbols, etc. to parent nodes.
    class SynthesizedAttribute
        : public FlowAttribute {
    public:
      SynthesizedAttribute(llvm::LLVMContext &C) : FlowAttribute(C) {}

      SynthesizedAttribute(const SynthesizedAttribute &other)
          : FlowAttribute(other) {}

      SynthesizedAttribute(const SynthesizedAttribute &&other)
          : FlowAttribute(other) {}

      SynthesizedAttribute(const InheritedAttribute &inherited)
          : FlowAttribute(inherited) {}

      SynthesizedAttribute &operator=(const SynthesizedAttribute &other) {
        SynthesizedAttribute temp(other);
        std::swap(temp, *this);
        return *this;
      }

      SynthesizedAttribute &operator=(const SynthesizedAttribute &&other) {
        SynthesizedAttribute temp(other);
        std::swap(temp, *this);
        return *this;
      }

      SynthesizedAttribute &operator=(const InheritedAttribute &inherited) {
        SynthesizedAttribute temp(inherited);
        std::swap(temp, *this);
        return *this;
      }
    };

    typedef FlowAttributeManager<
      InheritedAttribute,
      SynthesizedAttribute
      > FlowAttributeManagerType;

    /// Entering each symbol
    class EnterSymbolVisitor : public SymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterSymbolVisitor(FlowAttributeManagerType &am)
	  : attributeManager(am) {}

      void visit(Module & sym) override;
      void visit(Function & sym) override;
    };

    /// Invoke the EnterSymbolVisitor upon entering a symbol.
    class EnterSymbolAction : public VisitAction<EnterSymbolVisitor> {
    public:
      EnterSymbolAction(FlowAttributeManagerType &attributeManager)
          : VisitAction<EnterSymbolVisitor>(attributeManager) {}
    };

    /// Leaving each symbol definition.
    class LeaveSymbolVisitor : public SymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveSymbolVisitor(FlowAttributeManagerType &am)
	  : attributeManager(am) {}

      void visit(Function & sym) override;
      void visit(GlobalVariable & sym) override;
    };

    /// Invoke the LeaveSymbolVisitor when exiting symbols.
    class LeaveSymbolAction : public VisitAction<LeaveSymbolVisitor> {
    public:
      LeaveSymbolAction(FlowAttributeManagerType &attributeManager)
          : VisitAction<LeaveSymbolVisitor>(attributeManager) {}
    };

    /// Entering each statement
    class EnterStatementVisitor : public ControlVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterStatementVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(Block & stmt) override;
      void visit(Goto & stmt) override;
    };

    /// Invoke the EnterStatementVisitor upon entry to a statement.
    class EnterStatementAction : public VisitAction<EnterStatementVisitor> {
    public:
      EnterStatementAction(FlowAttributeManagerType &attributeManager)
          : VisitAction<EnterStatementVisitor>(attributeManager) {}
    };

    /// Leaving each statement
    class LeaveStatementVisitor : public ControlVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveStatementVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(Goto & stmt) override;
      void visit(Return & stmt) override;
      void visit(IfElse & stmt) override;
      void visit(IfThen & stmt) override;
      void visit(DoWhile & stmt) override;
    };

    /// Invoke the LeaveStatementVisitor upon exiting statements.
    class LeaveStatementAction : public VisitAction<LeaveStatementVisitor> {
    public:
      LeaveStatementAction(FlowAttributeManagerType &attributeManager)
          : VisitAction<LeaveStatementVisitor>(attributeManager) {}
    };

    /// Entering each expression
    class EnterExpressionVisitor : public ValueVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterExpressionVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(TuplePointer & expr) override;
      void visit(Allocate & expr) override;
    };

    /// Invoke the EnterExpressionVisitor upon entering an expression.
    class EnterExpressionAction : public VisitAction<EnterExpressionVisitor> {
    public:
      EnterExpressionAction(FlowAttributeManagerType &attributeManager)
          : VisitAction<EnterExpressionVisitor>(attributeManager) {}
    };

    /// Leaving each expression
    class LeaveExpressionVisitor : public ValueVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveExpressionVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

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
      void visit(Load & expr) override;
      void visit(TuplePointer & expr) override;
      void visit(Phi & expr) override;
      void visit(Store & expr) override;
      void visit(FunctionCall & expr) override;
      void visit(GlobalVariable & expr) override;
      void visit(Constant & expr) override;
    };

    /// Invoke the LeaveExpressionVisitor upon leaving an expression.
    class LeaveExpressionAction : public VisitAction<LeaveExpressionVisitor> {
    public:
      LeaveExpressionAction(FlowAttributeManagerType &attributeManager)
          : VisitAction<LeaveExpressionVisitor>(attributeManager) {}
    };

    /// This is the flow for translating expressions.
    class LLVMCodegenExpressionFlow final : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardValueFlowGenerator,
      EnterExpressionAction,
      AttributeFlowInheritedToSynthesizedAction<
        LeaveExpressionAction,
        FlowAttributeManagerType
        >,
      NullAction,
      NullAction,
      AttributeFlowSynthesizedToInheritedAction<
        NullAction,
        FlowAttributeManagerType
        >
      > {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardValueFlowGenerator,
      EnterExpressionAction,
      AttributeFlowInheritedToSynthesizedAction<
        LeaveExpressionAction,
        FlowAttributeManagerType
        >,
      NullAction,
      NullAction,
      AttributeFlowSynthesizedToInheritedAction<
        NullAction,
        FlowAttributeManagerType
        >
      > BaseType;

    public:
      LLVMCodegenExpressionFlow(llvm::LLVMContext &C)
          : BaseType(InheritedAttribute(C),
                     SynthesizedAttribute(C)) {}

      LLVMCodegenExpressionFlow(FlowAttributeManagerType &am)
          : BaseType(am.getInheritedAttributePrototype(),
                     am.getSynthesizedAttributePrototype()) {}
    };

    /// This is the flow for translating statements.
    class LLVMCodegenFlow final : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ForwardFlowGenerator,
      EnterStatementAction,
      AttributeFlowInheritedToSynthesizedAction<
        LeaveStatementAction,
        FlowAttributeManagerType
        >,
      // Transfer synthesized to inherited before statements.
      AttributeFlowSynthesizedToInheritedAction<
        NullAction,
        FlowAttributeManagerType
        >,
      NullAction,
      NullAction,
      NullJoinAction,
      // Transfer synthesized to inherited before expressions.
      AttributeFlowSynthesizedToInheritedAction<
        NullAction,
        FlowAttributeManagerType
        >,
      NullAction,
      FlowAction<LLVMCodegenFlow, LLVMCodegenExpressionFlow>,
      NullAction> {
      typedef AttributeFlow<
        InheritedAttribute,
        SynthesizedAttribute,
        ForwardFlowGenerator,
	EnterStatementAction,
        AttributeFlowInheritedToSynthesizedAction<
          LeaveStatementAction,
          FlowAttributeManagerType
          >,
        AttributeFlowSynthesizedToInheritedAction<
          NullAction,
          FlowAttributeManagerType
        >,
	NullAction,
	NullAction,
	NullJoinAction,
        AttributeFlowSynthesizedToInheritedAction<
          NullAction,
          FlowAttributeManagerType
          >,
        NullAction,
        FlowAction<LLVMCodegenFlow, LLVMCodegenExpressionFlow>,
	NullAction> BaseType;

    public:
      LLVMCodegenFlow(llvm::LLVMContext &C)
          : BaseType(InheritedAttribute(C),
                     SynthesizedAttribute(C)) {
        expression().setParentFlow(this);
      }

      LLVMCodegenFlow(const FlowAttributeManagerType &am)
          : BaseType(am.getInheritedAttributePrototype(),
                     am.getSynthesizedAttributePrototype()) {
        expression().setParentFlow(this);
      }
    };

    /// This is the flow to codegen a module.
    class LLVMCodegenSymbolFlow final : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      SymbolFlowGenerator,
      EnterSymbolAction,
      AttributeFlowInheritedToSynthesizedAction<
        LeaveSymbolAction,
        FlowAttributeManagerType
        >,
      NullAction,
      NullAction,
      NullAction,
      FlowAction<LLVMCodegenSymbolFlow, LLVMCodegenFlow>
      > {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      SymbolFlowGenerator,
      EnterSymbolAction,
      AttributeFlowInheritedToSynthesizedAction<
        LeaveSymbolAction,
        FlowAttributeManagerType
        >,
      NullAction,
      NullAction,
      NullAction,
      FlowAction<LLVMCodegenSymbolFlow, LLVMCodegenFlow>
      > BaseType;

    public:
      LLVMCodegenSymbolFlow(llvm::LLVMContext &C)
          : BaseType(InheritedAttribute(C),
                     SynthesizedAttribute(C)) {
        statement().setParentFlow(this);
      }
    };

    llvm::Module *TheModule;
    llvm::Function *TheFunction;

    /// Translate an IR tree.
    void runImpl(ptr<Module> node) override;
    void runImpl(ptr<Function> node) override;
    void runImpl(ptr<Producer> node) override;
    void runImpl(ptr<Control> node) override;

    llvm::LLVMContext Context;

  public:
    LLVMCodegenFilter(void)
    : Filter(range(), range(), range()), TheModule(0) {}

    llvm::Module *getModule(void) const {
      checkInvariant(TheModule, "Null module");
      return TheModule;
    }
  };
}

#endif
