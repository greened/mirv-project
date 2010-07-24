#include "LLVM.hpp"

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Utility/Cast.hpp>

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
      builder()->CreateAlloca(llvmType, 0, name);
    }
    else {
      checkInvariant(TheModule, "No module for global variable");
      const llvm::Type *llvmType = getType(type);
      TheModule->getOrInsertGlobal(name, llvmType);
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
    TheType = llvm::IntegerType::get(Context, type->bitsize());
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<Symbol<Type<Floating> > >::const_type type) 
  {
    checkInvariant(type->bitsize() == 32
      || type->bitsize() == 64,
      "Unexpected floating type");
    TheType = type->bitsize() == 32 ?
      llvm::Type::getFloatTy(Context) :   
      llvm::Type::getDoubleTy(Context);
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<Symbol<Type<Array> > >::const_type type) 
  {
    // TODO: See about making some of these vector types.
    // Get the element type.
    type->getElementType()->accept(*this);

    const llvm::Type *elementType = TheType;

    // Construct series of LLVM ArrayTypes, one for each dimension.
    for (auto d = type->dimensionRBegin();
         d != type->dimensionREnd();
         ++d) {
      elementType = llvm::ArrayType::get(elementType, *d);
    }

    TheType = elementType;
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
    type->getReturnType()->accept(*this);
    const llvm::Type *returnType = TheType;

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

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<Symbol<Type<StructType> > >::const_type type)
  {
    std::vector<const llvm::Type *> memberTypes;
    for (auto m = type->memberBegin();
         m != type->memberEnd();
         ++m) {
      (*m)->accept(*this);
      memberTypes.push_back(TheType);
    }

    TheType = llvm::StructType::get(Context, memberTypes);
  }
  
  void LLVMCodegenFilter::
  EnterDeclSymbolVisitor::visit(ptr<Symbol<Module> >::type sym)
  {
    FlowAttribute inh(attributeManager.getInheritedAttribute());
    inh.createModule(sym->name());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::
  EnterDeclSymbolVisitor::visit(ptr<Symbol<Function> >::type sym)
  {
    FlowAttribute inh(attributeManager.getInheritedAttribute());
    inh.createFunction(sym->name(), sym->type());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::
  EnterDeclSymbolVisitor::visit(ptr<Symbol<Variable> >::type sym)
  {
    FlowAttribute inh(attributeManager.getInheritedAttribute());
    inh.createVariable(sym->name(), sym->type());
    attributeManager.setInheritedAttribute(inh);
  }
  
  void LLVMCodegenFilter::
  EnterStatementVisitor::visit(ptr<Statement<Block> >::type stmt)
  {
    FlowAttribute inh(attributeManager.getInheritedAttribute());
    inh.createBlock("B");
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
    FlowAttribute syn(attributeManager.getInheritedAttribute());

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
    FlowAttribute inh(attributeManager.getInheritedAttribute(), true);
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<Statement<Assignment> >::type stmt)
  {
    // TODO: Handle return assignment.
    // Forward flow visits rhs first, so it is at index 0.
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(0).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

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

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // Start a new block after the then block.
    llvm::BasicBlock *after = syn.createBlock("at");

    // Create the terminator for the if block.
    syn.builder()->SetInsertPoint(cond->getParent());
    syn.builder()->CreateCondBr(cond, thn, after);

    // Create the terminator for the then block.
    syn.builder()->SetInsertPoint(thn);
    syn.builder()->CreateBr(after);

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

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(0));

    // Start a new block after the else block.
    llvm::BasicBlock *after = syn.createBlock("ae");

    // Create the terminator for the if block.
    syn.builder()->SetInsertPoint(cond->getParent());
    syn.builder()->CreateCondBr(cond, thn, els);

    // Create the terminator for the then block.
    syn.builder()->SetInsertPoint(thn);
    syn.builder()->CreateBr(after);

    // Create the terminator for the else block.
    syn.builder()->SetInsertPoint(els);
    syn.builder()->CreateBr(after);

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
    llvm::BasicBlock *body = attributeManager.getSynthesizedAttribute(0).
      getBlock();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // Start a new block after the cond/body block.
    llvm::BasicBlock *after = syn.createBlock("ab");

    // Create the terminator for the cond/body block.
    syn.builder()->SetInsertPoint(body);
    syn.builder()->CreateCondBr(cond, body, after);

    syn.builder()->SetInsertPoint(after);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::
  LeaveExpressionVisitor::visit(ptr<Expression<Add> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      syn.builder()->CreateFAdd(rhs, lhs, "r") :
      syn.builder()->CreateAdd(rhs, lhs, "r");

    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Subtract> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      syn.builder()->CreateFSub(rhs, lhs, "r") :
      syn.builder()->CreateSub(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Multiply> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      syn.builder()->CreateFMul(rhs, lhs, "r") :
      syn.builder()->CreateMul(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Divide> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      syn.builder()->CreateFDiv(rhs, lhs, "r") :
      // TODO: Support unsigned division and exact division.
      syn.builder()->CreateSDiv(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Modulus> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      syn.builder()->CreateFRem(rhs, lhs, "r") :
      // TODO: Support unsigned remainder.
      syn.builder()->CreateSRem(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Negate> >::type expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(0));

    llvm::Value *value = op->getType()->isFloatingPoint() ?
      syn.builder()->CreateFNeg(op, "r") :
      syn.builder()->CreateNeg(op, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateAnd(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalOr> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateOr(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalNot> >::type expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(0));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateNot(op, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = syn.builder()->CreateAnd(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = syn.builder()->CreateOr(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::type expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(0));

    llvm::Value *value = syn.builder()->CreateNot(op, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LessThan> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOLT(rhs, lhs, "r") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSLT(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOLE(rhs, lhs, "r") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSLE(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Equal> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOEQ(rhs, lhs, "r") :
      syn.builder()->CreateICmpEQ(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<NotEqual> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpONE(rhs, lhs, "r") :
      syn.builder()->CreateICmpNE(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<GreaterThan> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOGT(rhs, lhs, "r") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSGT(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    FlowAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      syn.builder()->CreateFCmpOGE(rhs, lhs, "r") :
      // TODO: Handle unsigned.
      syn.builder()->CreateICmpSGE(rhs, lhs, "r");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::type expr)
  {
    FlowAttribute syn(attributeManager.getInheritedAttribute());

    // Get the alloca or global for this variable.
    llvm::Value *value = syn.getVariable(expr->getSymbol()->name());
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::operator()(ptr<Node<Base> >::type node)
  {
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
