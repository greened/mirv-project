// Test building of variables.
//
// STDOUT: fdef testfunc {
// STDOUT:    vdecl a int32 *
// STDOUT:    vdecl b int32 *
// STDOUT:    vdecl c int32 *
// STDOUT:    {
// STDOUT:       allocate int32
// STDOUT:          vref a
// STDOUT:          cref int64 1
// STDOUT:       allocate int32
// STDOUT:          vref b
// STDOUT:          cref int64 1
// STDOUT:       allocate int32
// STDOUT:          vref c
// STDOUT:          cref int64 1
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Module;
using mirv::Function;
using mirv::IRBuilder;
using mirv::Type;
using mirv::FunctionType;
using mirv::ptr;
using mirv::PrintFilter;

namespace Builder = mirv::Builder;

using Builder::var;
using Builder::func;
using Builder::void_;
using Builder::int_;

int main(void)
{
  auto module = IRBuilder::GetOrCreateModule("testmodule");

  Builder::VariableTerminal a = {{"a"}};
  Builder::VariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};

  auto code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(
      module, ptr<Function>(),
      func["testfunc"].type[void_()] [
	var[a].type[int_(32)],
	var[b].type[int_(32)],
	var[c].type[int_(32)]
      ]
    );

  PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
