// Test building of function calls.
// STDOUT: fdef testfunc {
// STDOUT:    {
// STDOUT:       call
// STDOUT:          fref foo
// STDOUT:          load
// STDOUT:             cref int32 * & b
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
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

using Builder::var;
using Builder::func;
using Builder::module;
using Builder::void_;
using Builder::int_;
using Builder::do_;
using Builder::if_;

int main(void)
{
  auto module = IRBuilder::GetOrCreateModule("testmodule");

  auto inttype = IRBuilder::getIntegerType(32);

  auto functype = IRBuilder::getFunctionType(FunctionType::NotVararg,
                                             inttype);

  IRBuilder::GetOrCreateGlobalVariable("a", inttype);
  IRBuilder::GetOrCreateGlobalVariable("b", inttype);

  IRBuilder::GetOrCreateFunction("foo", functype);

  Builder::GlobalVariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};

  Builder::FunctionTerminal foo = {{"foo"}};

  auto code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(module,
      func["testfunc"].type[void_()] [
        foo(b)
      ]
    );

  PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
