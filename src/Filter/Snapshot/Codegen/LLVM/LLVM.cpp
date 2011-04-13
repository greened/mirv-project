#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Utility/Cast.hpp>

#include "LLVM.hpp"

namespace mirv {
  void LLVMCodegenFilter::
  FlowAttribute::
  createFunction(const std::string &name,
                 ptr<Symbol<Type<TypeBase> > >::const_type type)
  {
    checkInvariant(TheFunction == 0, "Function already exists");
    const llvm::FunctionType *llvmFunctionType =
      llvm::cast<const llvm::FunctionType>(getType(type));
    TheFunction = llvm::Function::Create(llvmFunctionType,
                                         // FIXME: Handle linkage.
                                         llvm::GlobalValue::ExternalLinkage,
                                         name,
                                         TheModule);
  }

  void LLVMCodegenFilter::
  FlowAttribute::
  createVariable(const std::string &name,
                 ptr<Symbol<Type<TypeBase> > >::const_type type)
  {
    if (TheFunction) {
      const llvm::Type *llvmType = getType(type);
      llvm::Value *var = builder()->CreateAlloca(llvmType, 0, name);
      VariableMap::iterator pos;
      bool inserted;
      std::tie(pos, inserted) = FunctionMap->insert(std::make_pair(name, var));
      checkInvariant(inserted, "Variable already exists");
    }
    else {
      checkInvariant(TheModule, "No module for global variable");
      const llvm::Type *llvmType = getType(type);
      llvm::Value *var = TheModule->getOrInsertGlobal(name, llvmType);
      VariableMap::iterator pos;
      bool inserted;
      std::tie(pos, inserted) = ModuleMap->insert(std::make_pair(name, var));
      checkInvariant(inserted, "Variable already exists");
    }
  }

  llvm::Value *LLVMCodegenFilter::
  FlowAttribute::
  getVariable(const std::string &name)
  {
    if (FunctionMap) {
      auto v = FunctionMap->find(name);
      if (v != FunctionMap->end()) {
        return v->second;
      }
    }
    auto v = ModuleMap->find(name);
    checkInvariant(v != ModuleMap->end(), "Cannot find variable");
    return v->second;
  }

  const llvm::Type *LLVMCodegenFilter::
  FlowAttribute::
  getType(ptr<Symbol<Type<TypeBase> > >::const_type type) const
  {
    TypeCreator creator(*Context);
    type->accept(creator);
    return creator.type();
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<Symbol<Type<Integral> > >::const_type type) 
  {
    ptr<Expression<Reference<Constant<Base> > > >::type expr =
      safe_cast<Expression<Reference<Constant<Base> > > >(type->bitsize());
    ptr<Symbol<Constant<std::uint64_t> > >::type constant =
      safe_cast<Symbol<Constant<std::uint64_t> > >(expr->getSymbol());

    TheType = llvm::IntegerType::get(Context, constant->value());
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<Symbol<Type<Floating> > >::const_type type) 
  {
    ptr<Expression<Reference<Constant<Base> > > >::type expr =
      safe_cast<Expression<Reference<Constant<Base> > > >(type->bitsize());
    ptr<Symbol<Constant<std::uint64_t> > >::type constant =
      safe_cast<Symbol<Constant<std::uint64_t> > >(expr->getSymbol());

    checkInvariant(constant->value() == 32
                   || constant->value() == 64,
                   "Unexpected floating type");
    TheType = constant->value() == 32 ?
      llvm::Type::getFloatTy(Context) :   
      llvm::Type::getDoubleTy(Context);
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<Symbol<Type<Tuple> > >::const_type type) 
  {
    // TODO: See about making some of these vector types.

    if (type->isUniform()) {
      (*type->begin())->accept(*this);
      const llvm::Type *elementType = TheType;

      // Size must be an integer constant for LLVM.
      ptr<Expression<Reference<Constant<Base> > > >::const_type cref =
        safe_cast<const Expression<Reference<Constant<Base> > > >(type->bitsize());
      ptr<Symbol<Constant<std::uint64_t> > >::const_type constant =
        safe_cast<const Symbol<Constant<std::uint64_t> > >(cref->getSymbol());
      TheType = llvm::ArrayType::get(elementType, constant->value());
      return;
    }

    // Create a struct type.
    std::vector<const llvm::Type *> memberTypes;
    for (auto m = type->begin();
         m != type->end();
         ++m) {
      (*m)->accept(*this);
      memberTypes.push_back(TheType);
    }

    TheType = llvm::StructType::get(Context, memberTypes);
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<Symbol<Type<Pointer> > >::const_type type) 
  {
    type->getBaseType()->accept(*this);
    const llvm::Type *baseType = TheType;
    TheType = llvm::PointerType::getUnqual(baseType);
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<Symbol<Type<FunctionType> > >::const_type type) 
  {
    const llvm::Type *returnType = 0;
    if (type->getReturnType()) {
      type->getReturnType()->accept(*this);
      returnType = TheType;
    }
    else {
      returnType = llvm::Type::getVoidTy(Context);
    }

    std::vector<const llvm::Type *> parameterTypes;
    for (auto p = type->parameterBegin();
         p != type->parameterEnd();
         ++p) {
      (*p)->accept(*this);
      parameterTypes.push_back(TheType);
    }

    // TODO: Handle vararg.
    TheType = llvm::FunctionType::get(returnType, parameterTypes, false);
  }

  void LLVMCodegenFilter::
  EnterSymbolVisitor::visit(ptr<Symbol<Module> >::type sym)
  {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    inh.createModule(sym->name());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::
  EnterSymbolVisitor::visit(ptr<Symbol<Function> >::type sym)
  {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    inh.createFunction(sym->name(), sym->type());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::
  LeaveSymbolVisitor::visit(ptr<Symbol<Function> >::type sym)
  {
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

  void LLVMCodegenFilter::
  LeaveSymbolVisitor::visit(ptr<Symbol<Variable> >::type sym)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
    syn.createVariable(sym->name(), sym->type());
    attributeManager.setSynthesizedAttribute(syn);
  }
  
  void LLVMCodegenFilter::
  EnterStatementVisitor::visit(ptr<Statement<Block> >::type stmt)
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

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Before> >::type stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<Statement<Before> >::type stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<After> >::type stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<Statement<After> >::type stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Goto> >::type stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<Statement<Goto> >::type stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<Statement<Return> >::type stmt)
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
  EnterStatementVisitor::visit(ptr<Statement<Assignment> >::type stmt)
  {
    // Make lhs return an address.
    InheritedAttribute inh(attributeManager.getInheritedAttribute(), true);
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<Statement<Assignment> >::type stmt)
  {
    // TODO: Handle return assignment.

    // See LLVMCodeGenFlow::visit(...<Assignment>...) to see how we
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

  void LLVMCodegenFilter::
  LeaveStatementVisitor::visit(ptr<Statement<IfThen> >::type stmt)
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
  LeaveStatementVisitor::visit(ptr<Statement<IfElse> >::type stmt)
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
    // block we started before encuntering this statement.
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
          terminator->removeFromParent();
          terminator = 0;
        }
      }
    }
    if (!terminator) {
      syn.builder()->SetInsertPoint(cond->getParent());
      syn.builder()->CreateCondBr(cond, thn, els);
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
  LeaveStatementVisitor::visit(ptr<Statement<While> >::type stmt)
  {
    error("Unimplemented");
  }

  void LLVMCodegenFilter::
  LeaveStatementVisitor::visit(ptr<Statement<DoWhile> >::type stmt)
  {
    llvm::Instruction *cond = 
      llvm::cast<llvm::Instruction>(attributeManager.getSynthesizedAttribute(1).
                                    getValue());

    llvm::BasicBlock *bodybegin = attributeManager.getInheritedAttribute().
      getBlock();
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

  void LLVMCodegenFilter::
  LeaveExpressionVisitor::visit(ptr<Expression<Add> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      syn.builder()->CreateFAdd(rhs, lhs, "add") :
      syn.builder()->CreateAdd(rhs, lhs, "add");

    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Subtract> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      syn.builder()->CreateFSub(rhs, lhs, "sub") :
      syn.builder()->CreateSub(rhs, lhs, "sub");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Multiply> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      syn.builder()->CreateFMul(rhs, lhs, "mul") :
      syn.builder()->CreateMul(rhs, lhs, "mul");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Divide> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      syn.builder()->CreateFDiv(rhs, lhs, "div") :
      // TODO: Support unsigned division and exact division.
      syn.builder()->CreateSDiv(rhs, lhs, "div");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Modulus> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      syn.builder()->CreateFRem(rhs, lhs, "mod") :
      // TODO: Support unsigned remainder.
      syn.builder()->CreateSRem(rhs, lhs, "mod");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Negate> >::type expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(0));

    llvm::Value *value = op->getType()->isFloatingPoint() ?
      syn.builder()->CreateFNeg(op, "neg") :
      syn.builder()->CreateNeg(op, "neg");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateAnd(rhs, lhs, "and");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalOr> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateOr(rhs, lhs, "or");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalNot> >::type expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(0));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateNot(op, "not");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = syn.builder()->CreateAnd(rhs, lhs, "band");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = syn.builder()->CreateOr(rhs, lhs, "bor");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::type expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(0));

    llvm::Value *value = syn.builder()->CreateNot(op, "bnot");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LessThan> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOLT(rhs, lhs, "lt") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSLT(rhs, lhs, "lt");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOLE(rhs, lhs, "le") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSLE(rhs, lhs, "le");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Equal> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOEQ(rhs, lhs, "eq") :
      syn.builder()->CreateICmpEQ(rhs, lhs, "eq");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<NotEqual> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpONE(rhs, lhs, "ne") :
      syn.builder()->CreateICmpNE(rhs, lhs, "ne");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<GreaterThan> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOGT(rhs, lhs, "gt") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSGT(rhs, lhs, "gt");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOGE(rhs, lhs, "ge") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSGE(rhs, lhs, "ge");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::type expr)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());

    // Get the alloca or global for this variable.  This is an address
    // so we need to load it if required.
    llvm::Value *value = syn.getVariable(expr->getSymbol()->name());

    if (!attributeManager.getInheritedAttribute().generateAddress()) {
      value = attributeManager.getInheritedAttribute().builder()->
        CreateLoad(value, "vref");
    }
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::operator()(ptr<Node<Base> >::type node)
  {
    if (ptr<Symbol<Module> >::type s =
        boost::dynamic_pointer_cast<Symbol<Module> >(node)) {
      ptr<LLVMCodegenSymbolFlow>::type flow(new LLVMCodegenSymbolFlow());
      s->accept(*flow);
      TheModule =
        flow->getAttributeManager().getLastSynthesizedAttribute().getModule();
    }
    else {
      error("Can only codegen complete modules");
    }

    //if (ptr<Statement<Base> >::type s = dyn_cast<Statement<Base> >(node)) {
    if (ptr<Statement<Base> >::type s = boost::dynamic_pointer_cast<Statement<Base> >(node)) {
      ptr<LLVMCodegenFlow>::type flow(new LLVMCodegenFlow());
      s->accept(*flow);
    }
    else if (ptr<Expression<Base> >::type e = boost::dynamic_pointer_cast<Expression<Base> >(node)) {
      ptr<LLVMCodegenExpressionFlow>::type flow(new LLVMCodegenExpressionFlow());
      e->accept(*flow);
    }
  }
}
