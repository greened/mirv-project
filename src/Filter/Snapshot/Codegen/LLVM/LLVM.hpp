#ifndef mirv_Filter_Snapshot_Codegen_LLVM_hpp
#define mirv_Filter_Snapshot_Codegen_LLVM_hpp

#include <mirv/Core/Filter/AttributeFlow.hpp>
#include <mirv/Core/Filter/ConstSymbolVisitor.hpp>
#include <mirv/Core/Filter/ForwardFlow.hpp>
#include <mirv/Core/Filter/ExpressionFlow.hpp>
#include <mirv/Core/Filter/Filter.hpp>
#include <mirv/Core/Filter/FlowAction.hpp>
#include <mirv/Core/Filter/Action.hpp>
#include <mirv/Core/Filter/SymbolFlow.hpp>
#include <mirv/Core/IR/Node.hpp>

#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/BasicBlock.h>
#include <llvm/Instruction.h>
#include <llvm/Support/IRBuilder.h>

namespace mirv {
  /// This is a filter to translate from MIRV IR to LLVM IR.
  class LLVMCodegenFilter
      : public Filter<Node<Base> > {
  private:
    class FlowAttribute {
    private:
      llvm::LLVMContext *Context;
      ptr<llvm::IRBuilder<> >::type Builder;
      llvm::Module *TheModule;
      llvm::Function *TheFunction;
      llvm::BasicBlock *TheBlock;
      llvm::Value *TheValue;
      bool ReturnValue;
      bool GenerateAddress;

      typedef Map<std::string, llvm::Value *>::type VariableMap;
      ptr<VariableMap>::type ModuleMap;
      ptr<VariableMap>::type FunctionMap;
      
      class TypeCreator : public ConstSymbolVisitor {
      private:
        llvm::LLVMContext &Context;
        const llvm::Type *TheType;

      public:
        TypeCreator(llvm::LLVMContext &context) 
            : Context(context) {}

        virtual void visit(ptr<Symbol<Type<Integral> > >::const_type);
        virtual void visit(ptr<Symbol<Type<Floating> > >::const_type);
        virtual void visit(ptr<Symbol<Type<Tuple> > >::const_type);
        virtual void visit(ptr<Symbol<Type<Pointer> > >::const_type);
        virtual void visit(ptr<Symbol<Type<FunctionType> > >::const_type);

        const llvm::Type *type(void) const {
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
              GenerateAddress(false),
              ModuleMap(new VariableMap),
              FunctionMap(new VariableMap) {}

      FlowAttribute(const FlowAttribute &other, bool address) 
          : Context(other.Context),
              Builder(other.Builder),
              TheModule(other.TheModule),
              TheFunction(other.TheFunction),
              TheBlock(other.TheBlock),
              TheValue(other.TheValue),
              ReturnValue(other.ReturnValue),
              GenerateAddress(address),
              ModuleMap(other.ModuleMap),
              FunctionMap(other.FunctionMap) {}

      FlowAttribute(const FlowAttribute &other) 
          : Context(other.Context),
              Builder(other.Builder),
              TheModule(other.TheModule),
              TheFunction(other.TheFunction),
              TheBlock(other.TheBlock),
              TheValue(other.TheValue),
              ReturnValue(other.ReturnValue),
              GenerateAddress(false),
              ModuleMap(other.ModuleMap),
              FunctionMap(other.FunctionMap) {}

      void clearFunctionMap(void) {
        FunctionMap->clear();
      }

      ptr<llvm::IRBuilder<> >::type builder(void) const {
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

      const llvm::Type *
      getType(ptr<Symbol<Type<TypeBase> > >::const_type) const;

      bool generateAddress(void) const {
        return GenerateAddress;
      }

      void createModule(const std::string &name) {
        checkInvariant(TheModule == 0, "Module already exists");
        TheModule = new llvm::Module(name, *Context);
      }

      llvm::Module *getModule(void) const {
        checkInvariant(TheModule, "Null module");
        return TheModule;
      }

      void createFunction(const std::string &name,
                          ptr<Symbol<Type<TypeBase> > >::const_type type);

      void createVariable(const std::string &name,
                          ptr<Symbol<Type<TypeBase> > >::const_type type);

      llvm::Value *getVariable(const std::string &name);

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

    class InheritedAttribute 
        : public FlowAttribute {
    public:
      InheritedAttribute(void) = default;

      InheritedAttribute(const InheritedAttribute &inherited, bool address)
          : FlowAttribute(inherited, address) {}

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

  public:

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

      void visit(ptr<Symbol<Module> >::type sym);
      void visit(ptr<Symbol<Function> >::type sym);
    };

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

      void visit(ptr<Symbol<Function> >::type sym);
      // We put this here so it can set a synthesized attribute.
      void visit(ptr<Symbol<Variable> >::type sym);
    };

    class LeaveSymbolAction : public VisitAction<LeaveSymbolVisitor> {
    public:
      LeaveSymbolAction(FlowAttributeManagerType &attributeManager) 
          : VisitAction<LeaveSymbolVisitor>(attributeManager) {}
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

      void visit(ptr<Statement<Before> >::type stmt);
      void visit(ptr<Statement<After> >::type stmt);
      void visit(ptr<Statement<Goto> >::type stmt);
      void visit(ptr<Statement<Return> >::type stmt);
      void visit(ptr<Statement<Assignment> >::type stmt);
      void visit(ptr<Statement<IfElse> >::type stmt);
      void visit(ptr<Statement<IfThen> >::type stmt);
      void visit(ptr<Statement<While> >::type stmt);
      void visit(ptr<Statement<DoWhile> >::type stmt);
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
      ForwardExpressionFlowGenerator,
      NullAction,
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
      FlowAction<LLVMCodegenFlow, LLVMCodegenExpressionFlow>,
      NullAction>,
        public boost::enable_shared_from_this<LLVMCodegenFlow> {
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

    /// This is the flow to codegen a module.
    class LLVMCodegenSymbolFlow : public AttributeFlow<
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
      >,
         public boost::enable_shared_from_this<LLVMCodegenSymbolFlow> {
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
      LLVMCodegenSymbolFlow(void)
          : BaseType(InheritedAttribute()) {
        statement().setParentFlow(this);
      }
    };

    llvm::Module *TheModule;

  public:
    LLVMCodegenFilter(void) : Filter<Node<Base> >(), TheModule(0) {}

    /// Translate an IR tree.
    void operator()(ptr<Node<Base> >::type node);

    llvm::Module *getModule(void) const {
      checkInvariant(TheModule, "Null module");
      return TheModule;
    }
  };
}

#endif
