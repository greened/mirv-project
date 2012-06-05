#ifndef mirv_Filter_Snapshot_Codegen_LLVM_hpp
#define mirv_Filter_Snapshot_Codegen_LLVM_hpp

#include <mirv/Core/Filter/AttributeFlow.hpp>
#include <mirv/Core/Filter/ConstSymbolVisitor.hpp>
#include <mirv/Core/Filter/ConstForwardStatementFlow.hpp>
#include <mirv/Core/Filter/ConstExpressionFlow.hpp>
#include <mirv/Core/Filter/ConstSymbolFlow.hpp>
#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Filter/FlowAction.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/Filter/SymbolFlow.hpp>
#include <mirv/Core/IR/Node.hpp>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/BasicBlock.h>
#include <llvm/Instruction.h>
#include <llvm/LLVMContext.h>
#include <llvm/Support/IRBuilder.h>

namespace mirv {
  /// This is a filter to translate from MIRV IR to LLVM IR.
  class LLVMCodegenFilter : public Filter<Node<Base> > {
  public:
    class InheritedAttribute;

    /// This is the base of codegen inherited and synthesized
    /// attributes.  It contains all of the necessary context for
    /// creating IR constructs and references for holding codegen
    /// results for parent nodes to examine.
    class FlowAttribute {
    private:
      llvm::LLVMContext *Context;
      ptr<llvm::IRBuilder<> > Builder;
      llvm::Module *TheModule;
      llvm::Function *TheFunction;
      llvm::Function *ReferencedFunction;
      llvm::BasicBlock *TheBlock;
      llvm::Value *TheValue;
      bool ReturnValue;

      typedef Map<std::string, llvm::Value *>::type VariableMap;
      ptr<VariableMap> ModuleMap;
      ptr<VariableMap> FunctionMap;

      class TypeCreator : public ConstSymbolVisitor {
      private:
        llvm::LLVMContext &Context;
        llvm::Type *TheType;

      public:
        TypeCreator(llvm::LLVMContext &context)
            : Context(context) {}

        virtual void visit(ptr<const Symbol<Type<Integral> > >);
        virtual void visit(ptr<const Symbol<Type<Floating> > >);
        virtual void visit(ptr<const Symbol<Type<Tuple> > >);
        virtual void visit(ptr<const Symbol<Type<Pointer> > >);
        virtual void visit(ptr<const Symbol<Type<FunctionType> > >);

        llvm::Type *type(void) const {
          return TheType;
        }
      };

    protected:
      void setValueUnchecked(llvm::Value *v) {
        TheValue = v;
      }

    public:
      FlowAttribute(void)
          : Context(&llvm::getGlobalContext()),
              Builder(new llvm::IRBuilder<>(*Context)),
              TheModule(0),
              TheFunction(0),
              TheBlock(0),
              TheValue(0),
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
      getType(ptr<const Symbol<Type<TypeBase> > >) const;

      void createModule(const std::string &name) {
        checkInvariant(TheModule == 0, "Module already exists");
        TheModule = new llvm::Module(name, *Context);
      }

      llvm::Module *getModule(void) const {
        checkInvariant(TheModule, "Null module");
        return TheModule;
      }

      void createFunction(const std::string &name,
                          ptr<const Symbol<Type<TypeBase> > > type);

      void setReferencedFunction(llvm::Function *function) {
        ReferencedFunction = function;
      }
      
      llvm::Function *referencedFunction(void) const {
        return ReferencedFunction;
      }

      void createVariable(const std::string &name,
                          ptr<const Symbol<Type<TypeBase> > > type);

      void createAlloca(const std::string &name,
                        ptr<const Symbol<Type<TypeBase> > > type);

      void createGlobalVariable(ptr<const Symbol<GlobalVariable> > sym,
                                const InheritedAttribute &inh);

      template<typename ValueType>
      void createIntegerConstant(ptr<const Symbol<Type<TypeBase> > > type,
                                         ValueType value,
                                         bool isSigned) {
        llvm::Type *llvmType = getType(type);
        llvm::Value *constant = llvm::ConstantInt::get(llvmType, value, isSigned);
        setValue(constant);
      }

      template<typename ValueType>
      void createFloatingPointConstant(ptr<const Symbol<Type<TypeBase> > > type,
                                               ValueType value) {
        llvm::Type *llvmType = getType(type);
        llvm::Value *constant = llvm::ConstantFP::get(llvmType, value);
        setValue(constant);
      }

      llvm::Value *getVariable(ptr<const Symbol<Variable> > sym);
      llvm::Value *getGlobalVariable(const std::string &name);

      llvm::BasicBlock *createBlock(const std::string &name) {
        checkInvariant(TheFunction, "No function for block");
        llvm::BasicBlock *block = llvm::BasicBlock::Create(*Context,
                                                           name,
                                                           TheFunction);
        builder()->SetInsertPoint(block);
        TheBlock = block;
        return block;
      }
    };

    class SynthesizedAttribute;

    /// This is the inherited attribute for code generation.  It
    /// carries the necessary context and utility functions to create
    /// IR constructs.
    class InheritedAttribute 
        : public FlowAttribute {
    public:
      InheritedAttribute(void) = default;

      InheritedAttribute(const InheritedAttribute &inherited)
          : FlowAttribute(inherited) {}

      InheritedAttribute(const SynthesizedAttribute &synthesized) 
          : FlowAttribute(synthesized) {
        // Do not inherited this property.
        setValueUnchecked(0);
      }

      InheritedAttribute &operator=(const SynthesizedAttribute &synthesized) {
        InheritedAttribute temp(synthesized);
        std::swap(temp, *this);
        return *this;
      }
    };

    /// This is the symthesized attribute for the code generator.  It
    /// carrries result Values, Symbols, etc. to parent nodes.
    class SynthesizedAttribute 
        : public FlowAttribute {
    public:
      SynthesizedAttribute(void) = default;

      SynthesizedAttribute(const InheritedAttribute &inherited) 
          : FlowAttribute(inherited) {}

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
    class EnterSymbolVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterSymbolVisitor(FlowAttributeManagerType &am)
	  : attributeManager(am) {}

      void visit(ptr<const Symbol<Module> > sym);
      void visit(ptr<const Symbol<Function> > sym);
    };

    /// Invoke the EnterSymbolVisitor upon entering a symbol.
    class EnterSymbolAction : public VisitAction<EnterSymbolVisitor> {
    public:
      EnterSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<EnterSymbolVisitor>(attributeManager) {}
    };

    /// Leaving each symbol definition.
    class LeaveSymbolVisitor : public ConstSymbolVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      LeaveSymbolVisitor(FlowAttributeManagerType &am)
	  : attributeManager(am) {}

      void visit(ptr<const Symbol<Function> > sym);
      // We put this here so it can set a synthesized attribute.
      void visit(ptr<const Symbol<Variable> > sym);
      void visit(ptr<const Symbol<GlobalVariable> > sym);
    };

    /// Invoke the LeaveSymbolVisitor when exiting symbols.
    class LeaveSymbolAction : public VisitAction<LeaveSymbolVisitor> {
    public:
      LeaveSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveSymbolVisitor>(attributeManager) {}
    };

    /// Entering each statement
    class EnterStatementVisitor : public ConstStatementVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterStatementVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Statement<Block> > stmt);
      void visit(ptr<const Statement<Before> > stmt);
      void visit(ptr<const Statement<After> > stmt);
      void visit(ptr<const Statement<Goto> > stmt);
      void visit(ptr<const Statement<Allocate> > stmt);
    };

    /// Invoke the EnterStatementVisitor upon entry to a statement.
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

      void visit(ptr<const Statement<Before> > stmt);
      void visit(ptr<const Statement<After> > stmt);
      void visit(ptr<const Statement<Goto> > stmt);
      void visit(ptr<const Statement<Return> > stmt);
      void visit(ptr<const Statement<Phi> > stmt);
      void visit(ptr<const Statement<Store> > stmt);
      void visit(ptr<const Statement<Call> > stmt);
      void visit(ptr<const Statement<IfElse> > stmt);
      void visit(ptr<const Statement<IfThen> > stmt);
      void visit(ptr<const Statement<While> > stmt);
      void visit(ptr<const Statement<DoWhile> > stmt);
    };

    /// Invoke the LeaveStatementVisitor upon exiting statements.
    class LeaveStatementAction : public VisitAction<LeaveStatementVisitor> {
    public:
      LeaveStatementAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveStatementVisitor>(attributeManager) {}
    };

    /// Entering each expression
    class EnterExpressionVisitor : public ConstExpressionVisitor {
    private:
      FlowAttributeManagerType &attributeManager;

    public:
      EnterExpressionVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Expression<TuplePointer> > expr);
    };

    /// Invoke the EnterExpressionVisitor upon entering an expression.
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
      void visit(ptr<const Expression<Reference<Variable> > > expr);
      void visit(ptr<const Expression<Reference<GlobalVariable> > > expr);
      void visit(ptr<const Expression<Load> > expr);
      void visit(ptr<const Expression<TuplePointer> > expr);
      void visit(ptr<const Expression<Reference<Function> > > expr);
      void visit(ptr<const Expression<Reference<Constant<Base> > > > expr);
    };

    /// Invoke the LeaveExpressionVisitor upon leaving an expression.
    class LeaveExpressionAction : public VisitAction<LeaveExpressionVisitor> {
    public:
      LeaveExpressionAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveExpressionVisitor>(attributeManager) {}
    };

    /// This is the flow for translating expressions.
    class LLVMCodegenExpressionFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstForwardExpressionFlowGenerator,
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
      ConstForwardExpressionFlowGenerator,
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
      LLVMCodegenExpressionFlow()
          : BaseType(InheritedAttribute()) {}

      LLVMCodegenExpressionFlow(FlowAttributeManagerType &)
          : BaseType(InheritedAttribute()) {}
    };

    /// This is the flow for translating statements.
    class LLVMCodegenFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstForwardStatementFlowGenerator,
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
      FlowAction<LLVMCodegenFlow, LLVMCodegenExpressionFlow>,
      NullAction>,
        public boost::enable_shared_from_this<LLVMCodegenFlow> {
      typedef AttributeFlow<
        InheritedAttribute,
        SynthesizedAttribute,
        ConstForwardStatementFlowGenerator,
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
        FlowAction<LLVMCodegenFlow, LLVMCodegenExpressionFlow>,
	NullAction> BaseType;

    public:
      LLVMCodegenFlow()
          : BaseType(InheritedAttribute()) {
        expression().setParentFlow(this);
      }

      LLVMCodegenFlow(const FlowAttributeManagerType &)
          : BaseType(InheritedAttribute()) {
        expression().setParentFlow(this);
      }

      // We need to reverse the order in which we visit the assignment
      // operands.  This makes it easier to tell the lhs to generate
      // an address because with this flow we visit it first and can
      // set the address control upon assignment statement entry.  If
      // we visited it after the rhs we would need a complicated
      // action to run between expressions.  Since there is no
      // between-expression action when flowing through statements
      // (assignment is the only multiple-expression statement and we
      // don't want to special-case it) we solve the problem this way.
      void visit(ptr<const Statement<Store> > stmt) {
        this->doEnter(stmt);

        for (auto i = stmt->expressionBegin();
             i != stmt->expressionEnd();
             ++i) {
          this->doBeforeExpression(stmt, i);
          this->doExpression(stmt, i);
          this->doAfterExpression(stmt, i);
        }

        this->doLeave(stmt);
      }
    };

    /// This is the flow to codegen a module.
    class LLVMCodegenSymbolFlow : public AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstSymbolFlowGenerator,
      EnterSymbolAction,
      AttributeFlowInheritedToSynthesizedAction<
        LeaveSymbolAction,
        FlowAttributeManagerType
        >,
      NullAction,
      NullAction,
      NullAction,
      FlowAction<LLVMCodegenSymbolFlow, LLVMCodegenFlow>
      >,
         public boost::enable_shared_from_this<LLVMCodegenSymbolFlow> {
    private:
      typedef AttributeFlow<
      InheritedAttribute,
      SynthesizedAttribute,
      ConstSymbolFlowGenerator,
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
      LLVMCodegenSymbolFlow(void)
          : BaseType(InheritedAttribute()) {
        statement().setParentFlow(this);
      }
    };

    llvm::Module *TheModule;

    /// Translate an IR tree.
    void run(ptr<Node<Base> > node);
    void run(ptr<const Node<Base> > node);

  public:
    LLVMCodegenFilter(void)
    : Filter<Node<Base> >(NullDependence::begin(), NullDependence::end(),
                          NullDependence::begin(), NullDependence::end(),
                          NullDependence::begin(), NullDependence::end()),
        TheModule(0) {}

    llvm::Module *getModule(void) const {
      checkInvariant(TheModule, "Null module");
      return TheModule;
    }
  };
}

#endif
