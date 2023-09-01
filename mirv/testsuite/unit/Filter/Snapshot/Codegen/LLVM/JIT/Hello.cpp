// Test building of variables.
//
//
// STDOUT: Hello, World!

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/ConstantGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Filter/Snapshot/Codegen/LLVM/LLVM.hpp>
#include <mirv/Filter/Snapshot/Codegen/LLVM/JIT/JIT.hpp>

#include <boost/tuple/tuple.hpp>

#include <functional>
#include <iostream>

using mirv::Module;
using mirv::ptr;

namespace Builder = mirv::Builder;

using Builder::var;
using Builder::func;
using Builder::module;
using Builder::void_;
using Builder::int_;
using Builder::do_;
using Builder::if_;
using Builder::vararg;

int main(void)
{
  Builder::FunctionTerminal printf_ = {{"printf"}};

  ptr<Module> code =
    mirv::safe_cast<Module>(
      Builder::translateWithGrammar<Builder::ModuleBuilder>(
        module["testmodule"] [
          func["printf"].type[int_(32)(*int_(8), vararg)],

          func["testfunc"].type[void_()] [
            printf_("Hello, World!\n")
          ]
        ]
      ));

  mirv::LLVMCodegenFilter codegen;
  codegen.run(code);

  std::function<void(void)> testfunc;
  mirv::JITContextHandle JITContext;
  boost::tie(testfunc, JITContext) = mirv::compile<void(void)>(code, "testfunc");

  testfunc();

  return(0);
}
