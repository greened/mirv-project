#ifndef mirv_Filter_Snapshot_Codegen_LLVM_JIT_JIT_hpp
#define mirv_Filter_Snapshot_Codegen_LLVM_JIT_JIT_hpp

#include <mirv/Core/Memory/Heap.hpp>

namespace llvm {
  class ExecutionEngine;
}

namespace mirv {
  class Module;

  typedef ptr<llvm::ExecutionEngine> JITContextHandle;

  namespace detail {
    JITContextHandle
    doCompile(void * &function,
              ptr<Module> module,
              const std::string &functionName);
  }

  template<typename Function>
  std::pair<Function *, JITContextHandle>
  compile(ptr<Module> module, const std::string &functionName) {
    void *function;
    JITContextHandle handle(detail::doCompile(function, module, functionName));
    return std::make_pair(reinterpret_cast<Function *>(function), handle);
  }

  void compileAndRun(ptr<Module> module,
                     const std::string &functionName);
}

#endif
