#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Variable.hpp>
#include <mirv/Core/IR/Reference.hpp>

#include <mirv/Filter/Snapshot/Codegen/LLVM/JIT/JIT.hpp>
#include <mirv/Filter/Snapshot/Codegen/LLVM/LLVM.hpp>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>

#include <llvm/Support/TargetSelect.h>

namespace mirv {
  namespace {
    llvm::Module *codegen(ptr<Symbol<Module> >::type module) {
      LLVMCodegenFilter codegen;
      codegen(module);
      return codegen.getModule();
    }
  }

  namespace detail {
    JITContextHandle
    doCompile(void * &function, 
              ptr<Symbol<Module> >::type module,
              const std::string &functionName) {
      llvm::InitializeNativeTarget();

      llvm::Module *llvmModule = codegen(module);
      llvm::EngineBuilder builder(llvmModule);
      std::string JITError;
      llvm::ExecutionEngine *engine =
        builder.
        setEngineKind(llvm::EngineKind::JIT).
        setErrorStr(&JITError).
        create();
      if (engine == 0) {
        error(JITError);
      }
      checkInvariant(engine != 0, "Could not create JIT");
      llvm::Function *LLVMfunction = llvmModule->getFunction(functionName);
      checkInvariant(LLVMfunction != 0, "Could not find function to jit");
      void *result = engine->getPointerToFunction(LLVMfunction);
      function = result;
      return JITContextHandle(engine);
    }
  }

  void compileAndRun(ptr<Symbol<Module> >::type module,
                     const std::string &functionName) {
    llvm::InitializeNativeTarget();

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
