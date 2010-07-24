#include "LLVM.hpp"

#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Utility/Cast.hpp>

namespace mirv {
  LLVMCodegenFilter::SynthesizedAttribute::
  SynthesizedAttribute(const InheritedAttribute &inherited) 
      : Context(inherited.Context),
          Builder(inherited.Builder),
          TheModule(inherited.TheModule),
          TheFunction(inherited.TheFunction),
          value(inherited.TheValue),
          ReturnValue(inherited.ReturnValue) {}  

  void LLVMCodegenFilter::
  InheritedAttribute::createFunction(const std::string &name,
                                     ptr<Symbol<Type<TypeBase> > > type)
  {
    checkInvariant(Function == 0, "Function already exists");

    ptr<Symbol<Type<FunctionType> > > functionType = 
      safe_cast<Symbol<Type<FunctionType> > >(type);
        
    llvm::Type *returnType = getType(type->getReturnType());
    std::vector<const llvm::Type *> llvmParameterTypes;

    for(auto p = type->parametersBegin();
        p != type->parametersEnd();
        ++p) {
      llvmParameterTypes.push_back(getType(*p));
    }

    llvm::FunctionType *llvmFunctionType =
      llvm::FunctionType::get(llvmReturnType,
                              llvmParameterTypes);

    TheFunction = llvm::Function::Create(llvmFunctionType,
                                         linkage,
                                         name,
                                         TheModule);
  }

  void LLVMCodegenFilter::
  InheritedAttribute::createVariable(const std::string &name,
                                     ptr<Symbol<Type<TypeBase> > > type)
  {
    if (TheFunction) {
      llvm::Type *llvmType = getType(type);
      builder()->CreateAlloca(llvmType, 0, name);
    }
    else {
      checkInvariant(TheModule, "No module for global variable");
      llvm::Type *llvmType = getType(type);
      TheModule->getOrInsertGlobal(name, llvmType);
    }
  }

  llvm::Type LLVMCodegenFilter::
  InheritedAttribute::getType(ptr<Symbol<Type<TypeBase> > >::type type) const
  {
    TypeCreator creator;
    type->accept(creator);
    return creator.type();
  }

  void LLVMCodegenFilter::InheritedAttribute::
  TypeCreator::visit(ptr<Symbol<Type<Integral> > >::type type) 
  {
    TheType = llvm::IntegerType::get(Context, type->bitsize());
  }

  void LLVMCodegenFilter::InheritedAttribute::
  TypeCreator::visit(ptr<Symbol<Type<Floating> > >::type type) 
  {
    checkInvariant(type->bitsize() == 32
      || type->bitsize() == 64,
      "Unexpected floating type");
    TheType = type->bitsize() == 32 ?
      llvm::Type::getFloatTy(Context) :   
      llvm::Type::getDoubleTy(Context);
  }

  void LLVMCodegenFilter::InheritedAttribute::
  TypeCreator::visit(ptr<Symbol<Type<Array> > >::type type) 
  {
    // TODO: See about making some of these vector types.
    // Get the element type.
    type->getElementType()->accept(*this);

    llvm::Type elementType = TheType;

    // Construct series of LLVM ArrayTypes, one for each dimension.
    for (auto d = type->dimensionsRBegin();
         d != type->dimensionsREnd();
         ++d) {
      elementType = llvm::ArrayType::get(elementType, *d);
    }

    TheType = elementType;
  }

  void LLVMCodegenFilter::InheritedAttribute::
  TypeCreator::visit(ptr<Symbol<Type<Pointer> > >::type type) 
  {
    type->getBaseType()->accept(*this);
    llvm::Type baseType = TheType;
    TheType = llvm::PointerType::getUnqual(baseType);
  }

  void LLVMCodegenFilter::InheritedAttribute::
  TypeCreator::visit(ptr<Symbol<Type<FunctionType> > >::type type) 
  {
    type->getReturnType()->accept(*this);
    llvm::Type returnType = TheType;

    std::vector<llvm::Type *> parameterTypes;
    for (auto p = type->parameterBegin();
         p != type->parameterEnd();
         ++p) {
      (*p)->accept(*this);
      parameterTypes.push_back(TheType);
    }

    // TODO: Handle vararg.
    TheType = llvm::FunctionType::get(returnType, parameterTypes, false);
  }

  void LLVMCodegenFilter::InheritedAttribute::
  TypeCreator::visit(ptr<Symbol<Type<StructType> > >::type type) 
  {
    std::vector<llvm::Type *> memberTypes;
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
    attributeManager.
      setInheritedAttribute(InheritedAttribute(attributeManager.
                                               getInheritedAttribute()).
                            createModule(sym->name()));
  }

  void LLVMCodegenFilter::
  EnterDeclSymbolVisitor::visit(ptr<Symbol<Function> >::type sym)
  {
    attributeManager.
      setInheritedAttribute(InheritedAttribute(attributeManager.
                                               getInheritedAttribute()).
                            createFunction(sym->name(), sym->type()));
  }

  void LLVMCodegenFilter::
  EnterDeclSymbolVisitor::visit(ptr<Symbol<Variable> >::type sym)
  {
    attributeManager.getInheritedAttribute().createVariable(sym->name(),
                                                            sym->type());
  }
  
  void LLVMCodegenFilter::
  EnterStatementVisitor::visit(ptr<Statement<Block> >::type stmt)
  {
    attributeManager.getInheritedAttribute().createBlock("Block");
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Before> >::type stmt)
  {
    error("unimplemented");
    attributeManager.getInheritedAttribute().createBlock("Before");
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<After> >::type stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Goto> >::type stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<Statement<Return> >::type stmt)
  {
    llvm::Value *inst = 
      attributeManager.getInheritedAttribute().hasReturnValue() ?
      builder()->CreateRet(attributeManager.
                           getInheritedAttribute().getValue()) :
      builder()->CreateRetVoid();

    attributeManager.setSynthesizedAttribute(SynthesizedAttribute(inst));
  }

  void LLVMCodegenFilter::
  EnterStatementVisitor::visit(ptr<Statement<Assignment> >::type stmt)
  {
    // Make lhs return an address.
    attributeManager.
      setInheritedAttribute(InheritedAttribute(attributeManager.
                                               getInheritedAttribute(), true));
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<Statement<Assignment> >::type stmt)
  {
    // TODO: Handle return assignment.
    // Forward flow visits rhs first, so it is at index 0.
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(0).getValue();

    // This assumes lhs is already an address.
    attributeMannager.getInheritedAttribute().builder()->CreateStore(rhs, lhs);
  }

  void LLVMCodegenFilter::
  LeaveExpressionVisitor::visit(ptr<Expression<Add> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      attributeMannager.getInheritedAttribute().builder()->
      CreateFAdd(rhs, lhs, "r") :
      attributeMannager.getInheritedAttribute().builder()->
      CreateAdd(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Subtract> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      attributeMannager.getInheritedAttribute().builder()->
      CreateFSub(rhs, lhs, "r") :
      attributeMannager.getInheritedAttribute().builder()->
      CreateSub(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Multiply> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *result = lhs->getType()->isFloatingPoint() ?
      attributeMannager.getInheritedAttribute().builder()->
      CreateFMul(rhs, lhs, "r") :
      attributeMannager.getInheritedAttribute().builder()->
      CreateMul(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Divide> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      attributeMannager.getInheritedAttribute().builder()->
      CreateFDiv(rhs, lhs, "r") :
      // TODO: Support unsigned division and exact division.
      attributeMannager.getInheritedAttribute().builder()->
      CreateSDiv(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Modulus> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      attributeMannager.getInheritedAttribute().builder()->
      CreateFRem(rhs, lhs, "r") :
      // TODO: Support unsigned remainder.
      attributeMannager.getInheritedAttribute().builder()->
      CreateSRem(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Negate> >::type expr)
  {
    llvm::Value *op = attributeMangager.getSynthesizedAttribute(0).getValue();
    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      attributeMannager.getInheritedAttribute().builder()->CreateFNeg(op, "r") :
      attributeMannager.getInheritedAttribute().builder()->CreateNeg(op, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = attributeMannager.getInheritedAttribute().builder()->
      CreateAnd(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalOr> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = attributeMannager.getInheritedAttribute().builder()->
      CreateOr(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalNot> >::type expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();
    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = attributeMannager.getInheritedAttribute().builder()->
      CreateNot(op, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = attributeMannager.getInheritedAttribute().builder()->
      CreateAnd(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = attributeMannager.getInheritedAttribute().builder()->
      CreateOr(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::type expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *value = attributeMannager.getInheritedAttribute().builder()->
      CreateNot(op, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LessThan> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      attributeMannager.getInheritedAttribute().builder()->
      CreateFCmpOLT(rhs, lhs, "r") :
      // TODO: Handle unsigned.
      attributeMannager.getInheritedAttribute().builder()->
      CreateICmpSLT(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      attributeMannager.getInheritedAttribute().builder()->
      CreateFCmpOLE(rhs, lhs, "r") :
      // TODO: Handle unsigned.
      attributeMannager.getInheritedAttribute().builder()->
      CreateICmpSLE(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Equal> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      attributeMannager.getInheritedAttribute().builder()->
      CreateFCmpOEQ(rhs, lhs, "r") :
      // TODO: Handle unsigned.
      attributeMannager.getInheritedAttribute().builder()->
      CreateICmpSEQ(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<NotEqual> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      attributeMannager.getInheritedAttribute().builder()->
      CreateFCmpONE(rhs, lhs, "r") :
      // TODO: Handle unsigned.
      attributeMannager.getInheritedAttribute().builder()->
      CreateICmpSNE(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<GreaterThan> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      attributeMannager.getInheritedAttribute().builder()->
      CreateFCmpOGT(rhs, lhs, "r") :
      // TODO: Handle unsigned.
      attributeMannager.getInheritedAttribute().builder()->
      CreateICmpSGT(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::type expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();
    llvm::Value *value = lhs->getType()->isFloatingPoint() ?
      // TODO: Handle unordered compares.
      attributeMannager.getInheritedAttribute().builder()->
      CreateFCmpOGE(rhs, lhs, "r") :
      // TODO: Handle unsigned.
      attributeMannager.getInheritedAttribute().builder()->
      CreateICmpSGE(rhs, lhs, "r");
    setValue(value);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::type expr)
  {
    // Get the alloca or global for this variable.
    llvm::Value *value = attributeManager.getInheritedAttribute().
      lookupVariable(expr->getSymbol()->getName());
    setValue(value);
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
