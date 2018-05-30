#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>

#include <mirv/Filter/Snapshot/Codegen/LLVM/JIT/JIT.hpp>
#include <mirv/Filter/Snapshot/Codegen/LLVM/LLVM.hpp>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>

#include <llvm/Support/TargetSelect.h>

namespace mirv {
  namespace {
    llvm::Module *codegen(ptr<Module> module) {
      LLVMCodegenFilter codegen;
      codegen.run(module);
      return codegen.getModule();
    }
  }

  namespace detail {
    JITContextHandle
    doCompile(void * &function,
              ptr<Module> module,
              const std::string &functionName) {
      llvm::InitializeNativeTarget();
      llvm::InitializeNativeTargetAsmPrinter();
      llvm::InitializeNativeTargetAsmParser();

      std::unique_ptr<llvm::Module> llvmModule(codegen(module));
      llvm::Function *LLVMfunction = llvmModule->getFunction(functionName);
      checkInvariant(LLVMfunction != 0, "Could not find function to jit");

      llvm::EngineBuilder builder(std::move(llvmModule));
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
      void *result = engine->getPointerToFunction(LLVMfunction);
      function = result;
      return JITContextHandle(engine);
    }
  }

  void compileAndRun(ptr<Module> module,
                     const std::string &functionName) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    std::unique_ptr<llvm::Module> llvmModule(codegen(module));
    llvm::Function *function = llvmModule->getFunction(functionName);
    checkInvariant(function != 0, "Could not find function to jit");

    llvm::EngineBuilder builder(std::move(llvmModule));
    llvm::ExecutionEngine *engine =
      builder.setEngineKind(llvm::EngineKind::JIT).create();
    engine->finalizeObject();
    std::vector<llvm::GenericValue> args;
    engine->runFunction(function, args);
    delete engine;
  }
}
