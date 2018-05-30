#include <mirv/Core/IR/Constants.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Filter/SymbolFlow.hpp>

#include <boost/mem_fn.hpp>

#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/CFG.h>

#include "LLVM.hpp"

#include <algorithm>

namespace mirv {
  namespace {
    class EnterConstantVisitor : public ValueVisitor {
    private:
      typedef LLVMCodegenFilter::FlowAttributeManagerType FlowAttributeManagerType;
      typedef LLVMCodegenFilter::InheritedAttribute InheritedAttribute;
      typedef LLVMCodegenFilter::SynthesizedAttribute SynthesizedAttribute;

      FlowAttributeManagerType &attributeManager;

      void visitIntegerConstant(IntegerConstant &sym, bool isSigned) {
        SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
        syn.createIntegerConstant(sym.getType(), sym.getValue(), isSigned);
        attributeManager.setSynthesizedAttribute(syn);
      }

      void visitFloatingPointConstant(FloatingPointConstant &sym) {
        SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
        syn.createFloatingPointConstant(sym.getType(), sym.getValue());
        attributeManager.setSynthesizedAttribute(syn);
      }

      void visitTupleConstant(TupleConstant &sym) {
        // TODO: Gather the tuple constants and create LLVM Constants
        // for them.

        //SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
        //syn.createFloatingPointConstant(sym.getType(),
        //sym.getValue());
        //attributeManager.setSynthesizedAttribute(syn);
      }

    public:
      EnterConstantVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(IntegerConstant &) override;
      void visit(FloatingPointConstant &) override;
      void visit(TupleConstant &) override;
    };

    class LeaveConstantVisitor : public ValueVisitor {
    private:
      typedef LLVMCodegenFilter::FlowAttributeManagerType FlowAttributeManagerType;
      typedef LLVMCodegenFilter::InheritedAttribute InheritedAttribute;
      typedef LLVMCodegenFilter::SynthesizedAttribute SynthesizedAttribute;

      //FlowAttributeManagerType &attributeManager;

      void visitTupleConstant(TupleConstant &sym) {
        // TODO: Synthesize a tuple constant from operands.

        //SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute());
        //attributeManager.setSynthesizedAttribute(syn);
      }

    public:
      LeaveConstantVisitor(FlowAttributeManagerType &am) {}
      //: attributeManager(am) {}

      // void visit(IntegerConstant &) override;
      // void visit(FloatingPointConstant &) override;
      // void visit(TupleConstant &) override;
    };

    class EnterConstantAction : public VisitAction<EnterConstantVisitor> {
    public:
      EnterConstantAction(LLVMCodegenFilter::
                          FlowAttributeManagerType &attributeManager)
          : VisitAction<EnterConstantVisitor>(attributeManager) {}
    };

    class ConstantFlow : public AttributeFlow<
      LLVMCodegenFilter::InheritedAttribute,
      LLVMCodegenFilter::SynthesizedAttribute,
      ForwardValueFlowGenerator,
      EnterConstantAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction> {
    private:
      typedef AttributeFlow<
      LLVMCodegenFilter::InheritedAttribute,
      LLVMCodegenFilter::SynthesizedAttribute,
      ForwardValueFlowGenerator,
      EnterConstantAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction> BaseType;

    public:
      ConstantFlow(LLVMCodegenFilter::InheritedAttribute &inherited,
                   LLVMCodegenFilter::SynthesizedAttribute &synthesized)
          : BaseType(inherited, synthesized) {}
    };

    void EnterConstantVisitor::visit(IntegerConstant &sym) {
      visitIntegerConstant(sym, true);
    }

    void EnterConstantVisitor::visit(FloatingPointConstant &sym) {
      visitFloatingPointConstant(sym);
    }

    void EnterConstantVisitor::visit(TupleConstant &sym)
    {
      // This should return the string value but it's not clear what
      // LLVM will do with that.  Typically we see this in global
      // variable initializers so do this there.  Abort if we ever get
      // here.
      error("Unimplemented");
      SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
      attributeManager.setSynthesizedAttribute(syn);
    }

    llvm::Constant *
    getConstant(Constant &constant,
                LLVMCodegenFilter::InheritedAttribute &inh,
                LLVMCodegenFilter::SynthesizedAttribute &syn) {
      ConstantFlow valueCreator(inh, syn);
      constant.accept(valueCreator);

      return llvm::cast<llvm::Constant>(valueCreator.getAttributeManager().
                                        getLastSynthesizedAttribute().
                                        getValue());
    }
  }

  void LLVMCodegenFilter::
  FlowAttribute::
  createFunction(const std::string &name,
                 ptr<const Type> type) {
    checkInvariant(TheModule != 0, "No current module");
    checkInvariant(TheFunction == 0, "Function already exists");
    llvm::FunctionType *llvmFunctionType =
      llvm::cast<llvm::FunctionType>(getType(type));
    TheFunction = llvm::Function::Create(llvmFunctionType,
                                         // FIXME: Handle linkage.
                                         llvm::GlobalValue::ExternalLinkage,
                                         name,
                                         TheModule);
  }

  void LLVMCodegenFilter::
  FlowAttribute::
  createVariable(const std::string &name,
                 ptr<const Type> type) {
    checkInvariant(TheFunction != 0, "No function to hold variable");
    //llvm::Type *llvmType = getType(type);
    VariableMap::iterator pos;
    bool inserted;
    std::tie(pos, inserted) = FunctionMap->insert(std::make_pair(name,
                                                                 nullptr));
    checkInvariant(inserted, "Variable already exists");
  }

  void LLVMCodegenFilter::
  FlowAttribute::
  createAlloca(const std::string &name,
               ptr<const Type> type) {
    checkInvariant(TheFunction != 0, "No function to hold variable");
    llvm::Type *llvmType = getType(type);
    llvm::Value *var = builder()->CreateAlloca(llvmType, 0, name);
    VariableMap::iterator pos = FunctionMap->find(name);
    checkInvariant(pos != FunctionMap->end(), "Cannot find variable");
    pos->second = var;
  }

  void LLVMCodegenFilter::
  FlowAttribute::
  createGlobalVariable(const GlobalVariable &sym,
                       const InheritedAttribute &inh,
                       const SynthesizedAttribute &syn)
  {
    const std::string &name = sym.getName();
    ptr<const Type> type(sym.getType());

    checkInvariant(TheModule, "No module for global variable");
    llvm::Type *llvmType = getType(type);

    if (const llvm::ArrayType *arrayType =
        llvm::dyn_cast<llvm::ArrayType>(llvmType)) {

      if (arrayType->getElementType() == inh.builder()->getInt8Ty()
          && sym.getInitializer()) {
        ptr<const TupleConstant> str = dyn_cast<const TupleConstant>(
            sym.getInitializer());

        if (str) {
          ptr<const TupleType> StrType =
            safe_cast<const TupleType>(str->getType());
          checkInvariant(StrType->isUniform(), "Array initializer not uniform");
          for (auto T : StrType->Types()) {
            ptr<const IntegerType> IType = safe_cast<const IntegerType>(T);
            auto Size = safe_cast<IntegerConstant>(IType->BitSize());
            checkInvariant(Size->getValue() == 8,
                           "String initializer not chars");
          }

          std::string StrValue;
          for (auto V : str->Operands()) {
            ptr<const IntegerConstant> IC = safe_cast<const IntegerConstant>(V);
            auto IType = safe_cast<const IntegerType>(IC->getType());
            auto Size = safe_cast<IntegerConstant>(IType->BitSize());
            checkInvariant(Size->getValue() == 8,
                           "String initializer not chars");
            StrValue.push_back(IC->getValue());
          }

          // This doesn't work because llvm's IRBuilder nonsensically
          // requires a basic block to exist here.  The code below
          // does the equivalent.

          //llvm::Value *pointer =
          //  inh.builder()->CreateGlobalString(str->getValue().c_str(), name);

          llvm::Constant *StrConstant =
            llvm::ConstantDataArray::getString(inh.builder()->getContext(),
                                               StrValue,
                                               true);
          llvm::Module &M = *inh.getModule();
          llvm::GlobalVariable *GV =
            new llvm::GlobalVariable(M, StrConstant->getType(),
                                     true, llvm::GlobalValue::InternalLinkage,
                                     StrConstant, "");
          GV->setName(name);
          llvm::Value *pointer = GV;

          VariableMap::iterator pos;
          bool inserted;
          std::tie(pos, inserted) =
            ModuleMap->insert(std::make_pair(name, pointer));
          checkInvariant(inserted, "Global variable already exists");
          return;
        }
      }
    }

    llvm::GlobalVariable *GV =
      new llvm::GlobalVariable(*TheModule, llvmType,
                               false, llvm::GlobalValue::InternalLinkage,
                               0, name);

    VariableMap::iterator pos;
    bool inserted;
    std::tie(pos, inserted) = ModuleMap->insert(std::make_pair(name, GV));
    checkInvariant(inserted, "Global variable already exists");
    if (sym.getInitializer()) {
      InheritedAttribute inherited(inh);
      SynthesizedAttribute synthesized(syn);
      GV->setInitializer(getConstant(*sym.getInitializer(),
                                     inherited,
                                     synthesized));
    }
  }

  llvm::Value *LLVMCodegenFilter::
  FlowAttribute::
  getGlobalVariable(const std::string &name) {
    auto v = ModuleMap->find(name);
    checkInvariant(v != ModuleMap->end(), "Cannot find variable");
    return v->second;
  }

  llvm::Type *LLVMCodegenFilter::
  FlowAttribute::getType(ptr<const Type> type) const
  {
    TypeCreator creator(Context);
    type->accept(creator);
    return creator.type();
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(const IntegerType &type)
  {
    ptr<ValueProducer> expr = type.BitSize();
    ptr<IntegerConstant> constant = safe_cast<IntegerConstant>(expr);

    TheType = llvm::IntegerType::get(Context, constant->getValue());
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(const FloatingPointType &type)
  {
    ptr<ValueProducer> expr = type.BitSize();

    ptr<IntegerConstant> constant = safe_cast<IntegerConstant>(expr);

    checkInvariant(constant->getValue() == 32
                   || constant->getValue() == 64,
                   "Unexpected floating type");
    TheType = constant->getValue() == 32 ?
      llvm::Type::getFloatTy(Context) :
      llvm::Type::getDoubleTy(Context);
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(const TupleType &type) {
    // TODO: See about making some of these vector types.
    std::vector<llvm::Type *> memberTypes;
    for (auto Op : type.Types()) {
      Op->accept(*this);
      memberTypes.push_back(TheType);
    }

    if (type.isUniform()) {
      checkInvariant(memberTypes.size() == 1,
                     "Multiple types for uniform tuple type");
      llvm::Type *elementType = memberTypes.front();

      // Size must be an integer constant for LLVM.
      ptr<ValueProducer> cref = type.Count();
      ptr<IntegerConstant> constant =
        safe_cast<IntegerConstant>(cref);
      TheType = llvm::ArrayType::get(elementType, constant->getValue());
      return;
    }

    // Create a struct type.
    TheType = llvm::StructType::get(Context, memberTypes);
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(const PointerType &type) {
    type.getBaseType()->accept(*this);
    llvm::Type *baseType = TheType;
    TheType = llvm::PointerType::getUnqual(baseType);
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(const FunctionType &type) {
    llvm::Type *returnType = 0;
    if (type.getReturnType()) {
      type.getReturnType()->accept(*this);
      returnType = TheType;
    }
    else {
      returnType = llvm::Type::getVoidTy(Context);
    }

    std::vector<llvm::Type *> parameterTypes;
    for (auto p : type.ParameterTypes()) {
      p->accept(*this);
      parameterTypes.push_back(TheType);
    }

    TheType = llvm::FunctionType::get(returnType,
                                      parameterTypes,
                                      type.isVararg());
  }

  void LLVMCodegenFilter::
  EnterSymbolVisitor::visit(Module &sym) {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    inh.createModule(sym.getName());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::
  EnterSymbolVisitor::visit(Function &sym)
  {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    inh.createFunction(sym.getName(), sym.getType());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::
  LeaveSymbolVisitor::visit(Function &sym)
  {
    if (sym.getBlock()) {
      SynthesizedAttribute syn(attributeManager.getLastSynthesizedAttribute());
      // Add a return if necessary.
      llvm::BasicBlock *body = syn.getBlock();
      if (!body->getTerminator()) {
        syn.builder()->SetInsertPoint(body);
        syn.builder()->CreateRetVoid();
      }

      syn.clearFunctionMap();
      attributeManager.setSynthesizedAttribute(syn);
    }
  }

  void LLVMCodegenFilter::
  LeaveSymbolVisitor::visit(GlobalVariable &sym)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
    syn.createGlobalVariable(sym,
                             attributeManager.getInheritedAttribute(),
                             syn);
    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::
  EnterStatementVisitor::visit(Block &stmt)
  {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());

    llvm::BasicBlock *prevBlock = inh.hasBlock() ? inh.getBlock() : 0;

    llvm::BasicBlock *thisBlock = inh.createBlock("B");

    if (prevBlock) {
      llvm::TerminatorInst *terminator = prevBlock->getTerminator();
      checkInvariant(!terminator, "Unexpected terminator");
      inh.builder()->SetInsertPoint(prevBlock);
      inh.builder()->CreateBr(thisBlock);
      inh.builder()->SetInsertPoint(thisBlock);
    }

    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(Goto &stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(Goto &stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(Return &stmt)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());

    llvm::Value *inst =
      attributeManager.getInheritedAttribute().hasReturnValue() ?
      syn.builder()->
      CreateRet(attributeManager.getInheritedAttribute().getValue()) :
      syn.builder()->CreateRetVoid();

    syn.setValue(inst);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::
  LeaveStatementVisitor::visit(IfThen &stmt)
  {
    llvm::Instruction *cond =
      llvm::cast<llvm::Instruction>(attributeManager.getSynthesizedAttribute(0).
                                    getValue());
    llvm::BasicBlock *thn = attributeManager.getSynthesizedAttribute(1).
      getBlock();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // Start a new block after the then block.  This continues the
    // block we started before this statement.
    llvm::BasicBlock *after = syn.createBlock("at");

    // Create the terminator for the if block.
    llvm::TerminatorInst *terminator = cond->getParent()->getTerminator();
    if (!terminator) {
      // If this is an unconditional branch pointing to the then
      // block, make it point to the after block.
      if (llvm::BranchInst *branch =
          llvm::dyn_cast<llvm::BranchInst>(terminator)) {
        if (branch->isUnconditional() && branch->getSuccessor(0) == thn) {
          branch->setSuccessor(0, after);
        }
      }
    } else {
      syn.builder()->SetInsertPoint(cond->getParent());
      syn.builder()->CreateCondBr(cond, thn, after);
    }

    // Create the terminator for the then block.  If there already is
    // a terminator it must be the result of a return, goto or other
    // branching behavior we don't want to disturb.

    // FIXME: What if the user inserted a goto to the then block?  We
    // don't want to assume structured control flow here because we
    // want to do debug dumps.
    terminator = thn->getTerminator();
    if (!terminator) {
      syn.builder()->SetInsertPoint(thn);
      syn.builder()->CreateBr(after);
    }

    syn.builder()->SetInsertPoint(after);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::
  LeaveStatementVisitor::visit(IfElse &stmt)
  {
    llvm::Instruction *cond =
      llvm::cast<llvm::Instruction>(attributeManager.getSynthesizedAttribute(0).
                                    getValue());
    llvm::BasicBlock *thn = attributeManager.getSynthesizedAttribute(1).
      getBlock();
    llvm::BasicBlock *els = attributeManager.getSynthesizedAttribute(2).
      getBlock();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(0));

    // Start a new block after the else block.  This continues the
    // block we started before encountering this statement.
    llvm::BasicBlock *after = syn.createBlock("ae");

    // Create the terminator for the if block.
    llvm::TerminatorInst *terminator = cond->getParent()->getTerminator();
    if (terminator) {
      // If this is an unconditional branch pointing to the then
      // block, make it a conditional branch pointing to the then and
      // else blocks.

      // FIXME: What if the user inserted a goto to the then block?  We
      // don't want to assume structured control flow here because we
      // want to do debug dumps.
      if (llvm::BranchInst *branch =
          llvm::dyn_cast<llvm::BranchInst>(terminator)) {
        if (branch->isUnconditional() && branch->getSuccessor(0) == thn) {
          // We can't make an unconditional branch into a conditional
          // one so just remove it.
          syn.builder()->SetInsertPoint(cond->getParent());
          syn.builder()->CreateCondBr(cond, thn, els);
          terminator->eraseFromParent();
          terminator = 0;
        }
      }
    }

    // Create the terminator for the then block.
    terminator = thn->getTerminator();
    if (terminator) {
      // If this is an unconditional branch pointing to the else
      // block, make it point to the after block.

      // FIXME: What if the user inserted a goto to the else block?  We
      // don't want to assume structured control flow here because we
      // want to do debug dumps.
      if (llvm::BranchInst *branch =
          llvm::dyn_cast<llvm::BranchInst>(terminator)) {
        if (branch->isUnconditional() && branch->getSuccessor(0) == els) {
          branch->setSuccessor(0, after);
        }
      }
    }
    else {
      syn.builder()->SetInsertPoint(thn);
      syn.builder()->CreateBr(after);
    }

    // Create the terminator for the else block.  If there already is
    // a terminator it must be the result of a return, goto or other
    // branching behavior we don't want to disturb.
    terminator = els->getTerminator();
    if (!terminator) {
      syn.builder()->SetInsertPoint(els);
      syn.builder()->CreateBr(after);
    }

    syn.builder()->SetInsertPoint(after);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::
  LeaveStatementVisitor::visit(DoWhile &stmt)
  {
    llvm::Instruction *cond =
      llvm::cast<llvm::Instruction>(attributeManager.getSynthesizedAttribute(1).
                                    getValue());

    llvm::BasicBlock *before = attributeManager.getInheritedAttribute().
      getBlock();

    checkInvariant(std::distance(succ_begin(before), succ_end(before)) == 1,
                   "More than one successor to loop entrence predecessor");

    llvm::BasicBlock *bodybegin = *succ_begin(before);
    llvm::BasicBlock *bodyend = attributeManager.getSynthesizedAttribute(0).
      getBlock();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // Start a new block after the cond/body block.
    llvm::BasicBlock *after = syn.createBlock("adw");

    // Create the terminator for the cond/body block.  If there
    // already is a terminator it must be the result of a return, goto
    // or other branching behavior we don't want to disturb.
    llvm::TerminatorInst *terminator = bodyend->getTerminator();
    if (!terminator) {
      syn.builder()->SetInsertPoint(bodyend);
      syn.builder()->CreateCondBr(cond, bodybegin, after);
    }

    syn.builder()->SetInsertPoint(after);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(Allocate &expt)
  {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    inh.createAlloca(expt.getName(),
                     safe_cast<const PointerType>(expt.getType())->
                     getBaseType());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::
  LeaveExpressionVisitor::visit(Add & expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatTy() ?
      syn.builder()->CreateFAdd(rhs, lhs, "add") :
      syn.builder()->CreateAdd(rhs, lhs, "add");

    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Subtract &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatTy() ?
      syn.builder()->CreateFSub(rhs, lhs, "sub") :
      syn.builder()->CreateSub(rhs, lhs, "sub");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Multiply &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatTy() ?
      syn.builder()->CreateFMul(rhs, lhs, "mul") :
      syn.builder()->CreateMul(rhs, lhs, "mul");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Divide &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatTy() ?
      syn.builder()->CreateFDiv(rhs, lhs, "div") :
      // TODO: Support unsigned division and exact division.
      syn.builder()->CreateSDiv(rhs, lhs, "div");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Modulus &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatTy() ?
      syn.builder()->CreateFRem(rhs, lhs, "mod") :
      // TODO: Support unsigned remainder.
      syn.builder()->CreateSRem(rhs, lhs, "mod");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Negate &expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(0));

    llvm::Value *value = op->getType()->isFloatTy() ?
      syn.builder()->CreateFNeg(op, "neg") :
      syn.builder()->CreateNeg(op, "neg");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(LogicalAnd &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateAnd(rhs, lhs, "and");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(LogicalOr &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateOr(rhs, lhs, "or");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(LogicalNot &expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(0));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateNot(op, "not");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(BitwiseAnd &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = syn.builder()->CreateAnd(rhs, lhs, "band");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(BitwiseOr &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = syn.builder()->CreateOr(rhs, lhs, "bor");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(BitwiseComplement &expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(0));

    llvm::Value *value = syn.builder()->CreateNot(op, "bnot");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(LessThan &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatTy() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOLT(rhs, lhs, "lt") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSLT(rhs, lhs, "lt");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(LessThanOrEqual &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatTy() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOLE(rhs, lhs, "le") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSLE(rhs, lhs, "le");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Equal &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatTy() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOEQ(rhs, lhs, "eq") :
      syn.builder()->CreateICmpEQ(rhs, lhs, "eq");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(NotEqual &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatTy() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpONE(rhs, lhs, "ne") :
      syn.builder()->CreateICmpNE(rhs, lhs, "ne");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(GreaterThan &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatTy() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOGT(rhs, lhs, "gt") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSGT(rhs, lhs, "gt");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(GreaterThanOrEqual &expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatTy() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOGE(rhs, lhs, "ge") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSGE(rhs, lhs, "ge");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  // void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Reference<Variable & > expr)
  // {
  //   SynthesizedAttribute syn(attributeManager.getInheritedAttribute());

  //   // Get the alloca for this variable.
  //   llvm::Value *value = syn.getVariable(expr->getSymbol());

  //   syn.setValue(value);

  //   attributeManager.setSynthesizedAttribute(syn);
  // }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(GlobalVariable &expr)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());

    // Get the global for this variable.  This is an address so we
    // need to load it if required.
    llvm::Value *value = syn.getGlobalVariable(expr.getName());

    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Load &expr)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());

    llvm::Value *address = attributeManager.getSynthesizedAttribute(0).getValue();

    syn.setValue(attributeManager.getInheritedAttribute().builder()->
                 CreateLoad(address, "load"));

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(TuplePointer &expr)
  {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(TuplePointer &expr)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());

    // LLVM expects a random-access iterator which these are not due
    // to the filter_iterator component.  So copy values to a
    // temporary vector.
    auto indicesBegin = attributeManager.begin();

    ++indicesBegin;

    std::vector<llvm::Value *> indices;

    std::copy(boost::make_transform_iterator(
                indicesBegin,
                boost::mem_fn(&SynthesizedAttribute::getValue)),
              boost::make_transform_iterator(
                attributeManager.end(),
                boost::mem_fn(&SynthesizedAttribute::getValue)),
              std::back_inserter(indices));

    llvm::Value *GEP = attributeManager.getInheritedAttribute().builder()->
      CreateGEP(attributeManager.begin()->getValue(),
                llvm::ArrayRef<llvm::Value *>(indices), "tptrgep");

    syn.setValue(GEP);
    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Phi &expt)
  {
    // TODO: Handle return assignment.

    // LLVM expects a random-access iterator which these are not due
    // to the filter_iterator component.  So copy values to a
    // temporary vector.
    std::vector<llvm::Value *>
      sources(boost::make_transform_iterator(
                attributeManager.begin(),
                boost::mem_fn(&SynthesizedAttribute::getValue)),
              boost::make_transform_iterator(
                attributeManager.end(),
                boost::mem_fn(&SynthesizedAttribute::getValue)));

    llvm::Value *phi = attributeManager.getInheritedAttribute().builder()->
      CreatePHI(attributeManager.begin()->getValue()->getType(), 0, "phi");

    // TODO: Add incoming values.

    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
    syn.setValue(phi);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Store &expt)
  {
    // TODO: Handle return assignment.

    // See LLVMCodeGenFlow::visit(...<Store>...) to see how we
    // changed the order of visitation from normal forward flow.  This
    // is why the lhs and rhs indices are reversed.
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // This assumes lhs is already an address.
    llvm::Value *inst = syn.builder()->CreateStore(rhs, lhs);
    syn.setValue(inst);
    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(FunctionCall &expt)
  {
    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    std::string name("name");

    // LLVM expects a random-access iterator which these are not due
    // to the filter_iterator component.  So copy values to a
    // temporary vector.
    auto argumentsBegin = attributeManager.begin();
    ++argumentsBegin;

    // See if we have a return value.
    llvm::Value *function = attributeManager.begin()->getValue();
    const llvm::PointerType *pointerType =
      llvm::cast<const llvm::PointerType>(function->getType());
    const llvm::FunctionType *functionType =
      llvm::cast<const llvm::FunctionType>(pointerType->getElementType());

    if (functionType->getReturnType()) {
      checkInvariant(argumentsBegin->getValue()->hasName(),
                     "No name for value");
      // Return value name.
      name = argumentsBegin->getValue()->getName();
      // Get to the first argument.
      ++argumentsBegin;
    }

    std::vector<llvm::Value *>
      arguments(boost::make_transform_iterator(
                  argumentsBegin,
                  boost::mem_fn(&SynthesizedAttribute::getValue)),
                boost::make_transform_iterator(
                  attributeManager.end(),
                  boost::mem_fn(&SynthesizedAttribute::getValue)));

    llvm::Value *Call = attributeManager.getInheritedAttribute().builder()->
      CreateCall(attributeManager.begin()->getValue(),
                 llvm::ArrayRef<llvm::Value *>(arguments), name);

    syn.setValue(Call);

    attributeManager.setSynthesizedAttribute(syn);
  }

  // void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Reference<Function & > expr)
  // {
  //   SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
  //   syn.setReferencedFunction(syn.getModule()->
  //                             getFunction(expr->getSymbol()->getName()));
  //   syn.setValue(syn.getModule()->getFunction(expr->getSymbol()->getName()));
  //   attributeManager.setSynthesizedAttribute(syn);
  // }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(Constant &expr)
  {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    SynthesizedAttribute syn(inh);
    syn.setValue(getConstant(expr, inh, syn));
    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::runImpl(ptr<Module> node) {
    std::unique_ptr<LLVMCodegenSymbolFlow>
      flow(new LLVMCodegenSymbolFlow(Context));
    node->accept(*flow);
    TheModule =
      flow->getAttributeManager().getLastSynthesizedAttribute().getModule();
  }

  void LLVMCodegenFilter::runImpl(ptr<Function> node) {
    std::unique_ptr<LLVMCodegenSymbolFlow>
      flow(new LLVMCodegenSymbolFlow(Context));
    node->accept(*flow);
    TheFunction =
      flow->getAttributeManager().getLastSynthesizedAttribute().getFunction();
  }

  void LLVMCodegenFilter::runImpl(ptr<Control> node) {
    std::unique_ptr<LLVMCodegenFlow> flow(new LLVMCodegenFlow(Context));
    node->accept(*flow);
  }

  void LLVMCodegenFilter::runImpl(ptr<Producer> node) {
    std::unique_ptr<LLVMCodegenExpressionFlow>
      flow(new LLVMCodegenExpressionFlow(Context));
    node->accept(*flow);
  }
}
