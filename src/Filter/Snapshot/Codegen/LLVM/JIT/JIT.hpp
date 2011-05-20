#ifndef mirv_Filter_Snapshot_Codegen_LLVM_JIT_JIT_hpp
#define mirv_Filter_Snapshot_Codegen_LLVM_JIT_JIT_hpp

#include <mirv/Core/IR/ModuleFwd.hpp>
#include <mirv/Core/IR/SymbolFwd.hpp>
#include <mirv/Core/Memory/Heap.hpp>

namespace mirv {
  void *JIT(ptr<Symbol<Module> >::type module,
            const std::string &functionName);
  void JITAndRun(ptr<Symbol<Module> >::type module,
                 const std::string &functionName);
}

#endif