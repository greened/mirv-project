#include <mirv/Core/IR/IR.hpp>
#include <mirv/Core/Utility/Cast.hpp>
#include <mirv/Core/Filter/ConstSymbolFlow.hpp>

#include <boost/mem_fn.hpp>

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/CFG.h>

#include "LLVM.hpp"

#include <algorithm>

namespace mirv {
  namespace {
    class EnterConstantVisitor : public ConstSymbolVisitor {
    private:
      typedef LLVMCodegenFilter::FlowAttributeManagerType FlowAttributeManagerType;
      typedef LLVMCodegenFilter::InheritedAttribute InheritedAttribute;
      typedef LLVMCodegenFilter::SynthesizedAttribute SynthesizedAttribute;

      FlowAttributeManagerType &attributeManager;

      template<typename ConstantType>
      void
      visitIntegerConstant(boost::shared_ptr<const Symbol<Constant<ConstantType> > > sym,
                           bool isSigned) {
        SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
        syn.createIntegerConstant(sym->type(), sym->value(), isSigned);
        attributeManager.setSynthesizedAttribute(syn);
      }

      template<typename ConstantType>
      void
      visitFloatingPointConstant(boost::shared_ptr<const Symbol<Constant<ConstantType> > > sym) {
        SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
        syn.createFloatingPointConstant(sym->type(), sym->value());
        attributeManager.setSynthesizedAttribute(syn);
      }

    public:
      EnterConstantVisitor(FlowAttributeManagerType &am)
          : attributeManager(am) {}

      void visit(ptr<const Symbol<Constant<std::int8_t> > >);
      void visit(ptr<const Symbol<Constant<std::uint8_t> > >);
      void visit(ptr<const Symbol<Constant<std::int16_t> > >);
      void visit(ptr<const Symbol<Constant<std::uint16_t> > >);
      void visit(ptr<const Symbol<Constant<std::int32_t> > >);
      void visit(ptr<const Symbol<Constant<std::uint32_t> > >);
      void visit(ptr<const Symbol<Constant<std::int64_t> > >);
      void visit(ptr<const Symbol<Constant<std::uint64_t> > >);
      void visit(ptr<const Symbol<Constant<float> > >);
      void visit(ptr<const Symbol<Constant<double> > >);
      void visit(ptr<const Symbol<Constant<std::string> > >);
      void visit(ptr<const Symbol<Constant<Address> > >);
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
      ConstSymbolFlowGenerator,
      EnterConstantAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction> {
    private:
      typedef AttributeFlow<
      LLVMCodegenFilter::InheritedAttribute,
      LLVMCodegenFilter::SynthesizedAttribute,
      ConstSymbolFlowGenerator,
      EnterConstantAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction,
      NullAction> BaseType;

    public:
      ConstantFlow(LLVMCodegenFilter::InheritedAttribute &inherited)
          : BaseType(inherited) {}
    };

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<std::int8_t> > > sym)
    {
      visitIntegerConstant(sym, true);
    }

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<std::uint8_t> > > sym)
    {
      visitIntegerConstant(sym, false);
    }

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<std::int16_t> > > sym)
    {
      visitIntegerConstant(sym, true);
    }

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<std::uint16_t> > > sym)
    {
      visitIntegerConstant(sym, false);
    }

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<std::int32_t> > > sym)
    {
      visitIntegerConstant(sym, true);
    }

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<std::uint32_t> > > sym)
    {
      visitIntegerConstant(sym, false);
    }

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<std::int64_t> > > sym)
    {
      visitIntegerConstant(sym, true);
    }

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<std::uint64_t> > > sym)
    {
      visitIntegerConstant(sym, false);
    }

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<float> > > sym)
    {
      visitFloatingPointConstant(sym);
    }

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<double> > > sym)
    {
      visitFloatingPointConstant(sym);
    }

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<std::string> > > sym)
    {
      // This should return the string value but it's not clear what
      // LLVM will do with that.  Typically we see this in global
      // variable initializers so do this there.  Abort if we ever get
      // here.
      error("Unimplemented");
      SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
      llvm::Value *pointer = syn.builder()->CreateGlobalString(sym->value().c_str());
      syn.setValue(pointer);
      attributeManager.setSynthesizedAttribute(syn);
    }

    void EnterConstantVisitor::visit(ptr<const Symbol<Constant<Address> > > sym)
    {
      SynthesizedAttribute syn(attributeManager.getInheritedAttribute());

      // First, see if this is a function address.
      if (llvm::Function *function =
          syn.getModule()->getFunction(sym->symbol()->name())) {
        error("Unimplemented");
      }
      else {
        // This must be a global variable address.  LLVM's
        // GlobalVariable is already an implicit address.
        syn.setValue(syn.getGlobalVariable(sym->symbol()->name()));
      }

      attributeManager.setSynthesizedAttribute(syn);
    }

    llvm::Constant *
    getConstant(ptr<const Expression<Reference<Constant<Base> > > > constant,
                LLVMCodegenFilter::InheritedAttribute &inh)
    {
      ConstantFlow valueCreator(inh);
      constant->getSymbol()->accept(valueCreator);

      return llvm::cast<llvm::Constant>(valueCreator.getAttributeManager().
                                        getLastSynthesizedAttribute().
                                        getValue());
    }
  }

  void LLVMCodegenFilter::
  FlowAttribute::
  createFunction(const std::string &name,
                 ptr<const Symbol<Type<TypeBase> > > type)
  {
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
                 ptr<const Symbol<Type<TypeBase> > > type)
  {
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
               ptr<const Symbol<Type<TypeBase> > > type)
  {
    checkInvariant(TheFunction != 0, "No function to hold variable");
    llvm::Type *llvmType = getType(type);
    llvm::Value *var = builder()->CreateAlloca(llvmType, 0, name);
    VariableMap::iterator pos = FunctionMap->find(name);
    checkInvariant(pos != FunctionMap->end(), "Cannot find variable");
    pos->second = var;
  }

  void LLVMCodegenFilter::
  FlowAttribute::
  createGlobalVariable(ptr<const Symbol<GlobalVariable> > sym,
                       const InheritedAttribute &inh)
  {
    const std::string &name = sym->name();
    ptr<const Symbol<Type<TypeBase> > > type(sym->type());

    checkInvariant(TheModule, "No module for global variable");
    llvm::Type *llvmType = getType(type);

    if (const llvm::ArrayType *arrayType =
        llvm::dyn_cast<llvm::ArrayType>(llvmType)) {
      
      if (arrayType->getElementType() == inh.builder()->getInt8Ty()
          && sym->initializer()) {
        ptr<const Symbol<Constant<std::string> > > str =
          dyn_cast<const Symbol<Constant<std::string> > >(
            sym->initializer()->getSymbol());
        if (str) {
          // This doesn't work because llvm's IRBuilder nonsensically
          // requires a basic block to exist here.  The code below
          // does the equivalent.

          // llvm::Value *pointer =
          //   inh.builder()->CreateGlobalString(str->value().c_str(), name);

          llvm::Constant *StrConstant =
            llvm::ConstantArray::get(inh.builder()->getContext(),
                                     str->value().c_str(), true);
          llvm::Module &M = *inh.getModule();
          llvm::GlobalVariable *GV =
            new llvm::GlobalVariable(M, StrConstant->getType(),
                                     true, llvm::GlobalValue::InternalLinkage,
                                     StrConstant, "", 0, false);
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
    if (sym->initializer()) {
      InheritedAttribute inherited(inh);
      GV->setInitializer(getConstant(sym->initializer(), inherited));
    }
  }

  llvm::Value *LLVMCodegenFilter::
  FlowAttribute::
  getVariable(ptr<const Symbol<Variable> > sym)
  {
    checkInvariant(FunctionMap, "No variables available");
    auto v = FunctionMap->find(sym->name());
    checkInvariant(v != FunctionMap->end(), "Cannot find variable");
    if (!v->second) {
      // We need an alloca for this.
      createAlloca(sym->name(), sym->type());
      v = FunctionMap->find(sym->name());
      checkInvariant(v->second, "Could not create alloca for temporary");
    }
    return v->second;
  }

  llvm::Value *LLVMCodegenFilter::
  FlowAttribute::
  getGlobalVariable(const std::string &name)
  {
    auto v = ModuleMap->find(name);
    checkInvariant(v != ModuleMap->end(), "Cannot find variable");
    return v->second;
  }

  llvm::Type *LLVMCodegenFilter::
  FlowAttribute::
  getType(ptr<const Symbol<Type<TypeBase> > > type) const
  {
    TypeCreator creator(*Context);
    type->accept(creator);
    return creator.type();
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<const Symbol<Type<Integral> > > type) 
  {
    ptr<Expression<Reference<Constant<Base> > > > expr =
      safe_cast<Expression<Reference<Constant<Base> > > >(type->bitsize());
    ptr<Symbol<Constant<std::uint64_t> > > constant =
      safe_cast<Symbol<Constant<std::uint64_t> > >(expr->getSymbol());

    TheType = llvm::IntegerType::get(Context, constant->value());
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<const Symbol<Type<Floating> > > type) 
  {
    ptr<Expression<Reference<Constant<Base> > > > expr =
      safe_cast<Expression<Reference<Constant<Base> > > >(type->bitsize());
    ptr<Symbol<Constant<std::uint64_t> > > constant =
      safe_cast<Symbol<Constant<std::uint64_t> > >(expr->getSymbol());

    checkInvariant(constant->value() == 32
                   || constant->value() == 64,
                   "Unexpected floating type");
    TheType = constant->value() == 32 ?
      llvm::Type::getFloatTy(Context) :   
      llvm::Type::getDoubleTy(Context);
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<const Symbol<Type<Tuple> > > type) 
  {
    // TODO: See about making some of these vector types.
    if (type->isUniform()) {
      (*type->begin())->accept(*this);
      llvm::Type *elementType = TheType;

      // Size must be an integer constant for LLVM.
      ptr<const Expression<Reference<Constant<Base> > > > cref =
        safe_cast<const Expression<Reference<Constant<Base> > > >(type->count());
      ptr<const Symbol<Constant<std::uint64_t> > > constant =
        safe_cast<const Symbol<Constant<std::uint64_t> > >(cref->getSymbol());
      TheType = llvm::ArrayType::get(elementType, constant->value());
      return;
    }

    // Create a struct type.
    std::vector<llvm::Type *> memberTypes;
    for (auto m = type->begin();
         m != type->end();
         ++m) {
      (*m)->accept(*this);
      memberTypes.push_back(TheType);
    }

    TheType = llvm::StructType::get(Context, memberTypes);
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<const Symbol<Type<Pointer> > > type) 
  {
    type->getBaseType()->accept(*this);
    llvm::Type *baseType = TheType;
    TheType = llvm::PointerType::getUnqual(baseType);
  }

  void LLVMCodegenFilter::FlowAttribute::
  TypeCreator::visit(ptr<const Symbol<Type<FunctionType> > > type) 
  {
    llvm::Type *returnType = 0;
    if (type->getReturnType()) {
      type->getReturnType()->accept(*this);
      returnType = TheType;
    }
    else {
      returnType = llvm::Type::getVoidTy(Context);
    }

    std::vector<llvm::Type *> parameterTypes;
    for (auto p = type->parameterBegin();
         p != type->parameterEnd();
         ++p) {
      (*p)->accept(*this);
      parameterTypes.push_back(TheType);
    }

    TheType = llvm::FunctionType::get(returnType,
                                      parameterTypes,
                                      type->isVararg());
  }

  void LLVMCodegenFilter::
  EnterSymbolVisitor::visit(ptr<const Symbol<Module> > sym)
  {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    inh.createModule(sym->name());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::
  EnterSymbolVisitor::visit(ptr<const Symbol<Function> > sym)
  {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    inh.createFunction(sym->name(), sym->type());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::
  LeaveSymbolVisitor::visit(ptr<const Symbol<Function> > sym)
  {
    if (!sym->statementEmpty()) {
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
  LeaveSymbolVisitor::visit(ptr<const Symbol<Variable> > sym)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
    syn.createVariable(sym->name(), sym->type());
    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::
  LeaveSymbolVisitor::visit(ptr<const Symbol<GlobalVariable> > sym)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
    syn.createGlobalVariable(sym, attributeManager.getInheritedAttribute());
    attributeManager.setSynthesizedAttribute(syn);
  }
  
  void LLVMCodegenFilter::
  EnterStatementVisitor::visit(ptr<const Statement<Block> > stmt)
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

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<const Statement<Before> > stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<const Statement<Before> > stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<const Statement<After> > stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<const Statement<After> > stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<const Statement<Goto> > stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<const Statement<Goto> > stmt)
  {
    error("unimplemented");
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<const Statement<Return> > stmt)
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

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<const Statement<Phi> > stmt)
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

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<const Statement<Store> > stmt)
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

  void LLVMCodegenFilter::EnterStatementVisitor::visit(ptr<const Statement<Allocate> > stmt)
  {
    // LHS must be a vref.
    ptr<const Symbol<Variable> > variable =
      safe_cast<const Expression<Reference<Variable> > >(stmt->getLeftExpression())->
      getSymbol();
  
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    inh.createAlloca(variable->name(),
                     safe_cast<const Symbol<Type<Pointer> > >(variable->type())->
                     getBaseType());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::LeaveStatementVisitor::visit(ptr<const Statement<Call> > stmt)
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


  void LLVMCodegenFilter::
  LeaveStatementVisitor::visit(ptr<const Statement<IfThen> > stmt)
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
  LeaveStatementVisitor::visit(ptr<const Statement<IfElse> > stmt)
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
  LeaveStatementVisitor::visit(ptr<const Statement<While> > stmt)
  {
    error("Unimplemented");
  }

  void LLVMCodegenFilter::
  LeaveStatementVisitor::visit(ptr<const Statement<DoWhile> > stmt)
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

  void LLVMCodegenFilter::
  LeaveExpressionVisitor::visit(ptr<const Expression<Add> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<Subtract> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<Multiply> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<Divide> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<Modulus> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<Negate> > expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(0));

    llvm::Value *value = op->getType()->isFloatTy() ?
      syn.builder()->CreateFNeg(op, "neg") :
      syn.builder()->CreateNeg(op, "neg");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<LogicalAnd> > expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateAnd(rhs, lhs, "and");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<LogicalOr> > expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateOr(rhs, lhs, "or");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<LogicalNot> > expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(0));

    // TODO: Do we require this to result in 1 if true?
    llvm::Value *value = syn.builder()->CreateNot(op, "not");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<BitwiseAnd> > expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = syn.builder()->CreateAnd(rhs, lhs, "band");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<BitwiseOr> > expr)
  {
    llvm::Value *lhs = attributeManager.getSynthesizedAttribute(0).getValue();
    llvm::Value *rhs = attributeManager.getSynthesizedAttribute(1).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(1));

    llvm::Value *value = syn.builder()->CreateOr(rhs, lhs, "bor");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<BitwiseComplement> > expr)
  {
    llvm::Value *op = attributeManager.getSynthesizedAttribute(0).getValue();

    SynthesizedAttribute syn(attributeManager.getSynthesizedAttribute(0));

    llvm::Value *value = syn.builder()->CreateNot(op, "bnot");
    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<LessThan> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<LessThanOrEqual> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<Equal> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<NotEqual> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<GreaterThan> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<GreaterThanOrEqual> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<Reference<Variable> > > expr)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());

    // Get the alloca for this variable.
    llvm::Value *value = syn.getVariable(expr->getSymbol());

    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<Reference<GlobalVariable> > > expr)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());

    // Get the global for this variable.  This is an address so we
    // need to load it if required.
    llvm::Value *value = syn.getGlobalVariable(expr->getSymbol()->name());

    syn.setValue(value);

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<Load> > expr)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());

    llvm::Value *address = attributeManager.getSynthesizedAttribute(0).getValue();

    syn.setValue(attributeManager.getInheritedAttribute().builder()->
                 CreateLoad(address, "load"));

    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::EnterExpressionVisitor::visit(ptr<const Expression<TuplePointer> > expr)
  {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    attributeManager.setInheritedAttribute(inh);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<TuplePointer> > expr)
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

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<Reference<Function> > > expr)
  {
    SynthesizedAttribute syn(attributeManager.getInheritedAttribute());
    syn.setReferencedFunction(syn.getModule()->
                              getFunction(expr->getSymbol()->name()));
    syn.setValue(syn.getModule()->getFunction(expr->getSymbol()->name()));
    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::LeaveExpressionVisitor::visit(ptr<const Expression<Reference<Constant<Base> > > > expr)
  {
    InheritedAttribute inh(attributeManager.getInheritedAttribute());
    SynthesizedAttribute syn(inh);
    syn.setValue(getConstant(expr, inh));
    attributeManager.setSynthesizedAttribute(syn);
  }

  void LLVMCodegenFilter::operator()(ptr<Node<Base> > node)
  {
    if (ptr<Symbol<Module> > s =
        boost::dynamic_pointer_cast<Symbol<Module> >(node)) {
      ptr<LLVMCodegenSymbolFlow> flow(new LLVMCodegenSymbolFlow());
      s->accept(*flow);
      TheModule =
        flow->getAttributeManager().getLastSynthesizedAttribute().getModule();
    }
    else {
      error("Can only codegen complete modules");
    }

    //if (ptr<const Statement<Base> > s = dyn_cast<Statement<Base> >(node)) {
    if (ptr<Statement<Base> > s = boost::dynamic_pointer_cast<Statement<Base> >(node)) {
      ptr<LLVMCodegenFlow> flow(new LLVMCodegenFlow());
      s->accept(*flow);
    }
    else if (ptr<Expression<Base> > e = boost::dynamic_pointer_cast<Expression<Base> >(node)) {
      ptr<LLVMCodegenExpressionFlow> flow(new LLVMCodegenExpressionFlow());
      e->accept(*flow);
    }
  }
}
