// Test building of functions.
//
// STDOUT: fdef testfunc {
// STDOUT:    {
// STDOUT:       store
// STDOUT:          cref int32 * & a
// STDOUT:          +
// STDOUT:             load
// STDOUT:                cref int32 * & a
// STDOUT:             load
// STDOUT:                cref int32 * & b
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Builder/FunctionGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Module;
using mirv::Function;
using mirv::GlobalVariable;
using mirv::IRBuilder;
using mirv::Type;
using mirv::FunctionType;
using mirv::ptr;
using mirv::PrintFilter;

namespace Builder = mirv::Builder;

using Builder::func;
using Builder::void_;

int main(void)
{
  auto module = IRBuilder::GetOrCreateModule("testmodule");

  auto inttype = IRBuilder::getIntegerType(32);

  IRBuilder::GetOrCreateGlobalVariable("a", inttype);
  IRBuilder::GetOrCreateGlobalVariable("b", inttype);

  Builder::GlobalVariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};

  auto code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(module,
      func["testfunc"].type[void_()] [
       	a = a + b
      ]
    );

  PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
