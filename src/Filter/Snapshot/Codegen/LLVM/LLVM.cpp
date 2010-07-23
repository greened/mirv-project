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
      setInheritedAttribute(InheritedAttribute(getInheritedAttribute()).
                            createModule(sym->name()));
  }

  void LLVMCodegenFilter::
  EnterDeclSymbolVisitor::visit(ptr<Symbol<Function> >::type sym)
  {
    attributeManager.
      setInheritedAttribute(InheritedAttribute(getInheritedAttribute()).
                            createFunction(sym->name(), sym->type()));
  }

  void LLVMCodegenFilter::
  EnterDeclSymbolVisitor::visit(ptr<Symbol<Variable> >::type sym)
  {
    getInheritedAttribute().createVariable(sym->name(), sym->type());
  }
  
  void LLVMCodegenFilter::
  EnterStatementVisitor::visit(ptr<Statement<Block> >::type stmt)
  {
    getInheritedAttribute().createBlock("Block");
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<Statement<Before> >::type stmt)
  {
    error("unimplemented");
    getInheritedAttribute().createBlock("Before");
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
    llvm::Value *inst = getInheritedAttribute().hasReturnValue() ?
      builder()->CreateRet(getInheritedAttribute().getValue()) :
      builder()->CreateRetVoid();

    setSynthesizedAttribute(SynthesizedAttribute(inst));
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<Statement<Assignment> >::type stmt)
  {
    // TODO: Handle return assignment.
    // Forward flow visits rhs first, so it is at index 0.
    llvm::Value *lhs = getSynthesizedAttribute(1).getValue();
    llvm::Value *rhs = getSynthesizedAttribute(0).getValue();

    // This assumes lhs is already an address.
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Add> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Subtract> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Multiply> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Divide> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Modulus> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Negate> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalAnd> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalOr> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LogicalNot> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseAnd> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseOr> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<BitwiseComplement> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LessThan> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<LessThanOrEqual> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Equal> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<NotEqual> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<GreaterThan> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<GreaterThanOrEqual> >::type expr)
  {
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<Expression<Reference<Variable> > >::type expr)
  {
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
