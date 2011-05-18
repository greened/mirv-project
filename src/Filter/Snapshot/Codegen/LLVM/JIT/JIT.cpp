#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Variable.hpp>

#include <mirv/Filter/Snapshot/Codegen/LLVM/JIT/JIT.hpp>
#include <mirv/Filter/Snapshot/Codegen/LLVM/LLVM.hpp>

#include <llvm/ExecutionEngine/ExecutionEngine.h>

namespace mirv {
  namespace {
    llvm::Module *codegen(ptr<Symbol<Module> >::type module) {
      LLVMCodegenFilter codegen;
      codegen(module);
      return codegen.getModule();
    }
  }

  void *JIT(ptr<Symbol<Module> >::type module,
            const std::string &functionName) {
    llvm::Module *llvmModule = codegen(module);
    llvm::EngineBuilder builder(llvmModule);
    llvm::ExecutionEngine *engine =
      builder.setEngineKind(llvm::EngineKind::JIT).create();
    llvm::Function *function = llvmModule->getFunction(functionName);
    checkInvariant(function != 0, "Could not find function to jit");
    void *result = engine->getPointerToFunctionOrStub(function);
    delete engine;
    return result;
  }

  void JITAndRun(ptr<Symbol<Module> >::type module,
                 const std::string &functionName) {
    llvm::Module *llvmModule = codegen(module);
    llvm::EngineBuilder builder(llvmModule);
    llvm::ExecutionEngine *engine =
      builder.setEngineKind(llvm::EngineKind::JIT).create();
    llvm::Function *function = llvmModule->getFunction(functionName);
    checkInvariant(function != 0, "Could not find function to jit");
    engine->runJITOnFunction(function);
    delete engine;
  }
}
