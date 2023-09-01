// Test building of function calls combined with another statement.
// STDOUT: fdef testfunc {
// STDOUT:    vdecl __ct0__ int32
// STDOUT:    {
// STDOUT:       store
// STDOUT:          cref int32 * & b
// STDOUT:          load
// STDOUT:             cref int32 * & a
// STDOUT:       call
// STDOUT:          fref foo
// STDOUT:          vref __ct0__
// STDOUT:          load
// STDOUT:             cref int32 * & b
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::IRBuilder;
using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::FunctionType;
using mirv::GlobalVariable;
using mirv::Type;
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
  ptr<Module> module = IRBuilder::GetOrCreateModule("testmodule");

  // ptr<const Type> inttype = IRBuilder::getIntegerType(32);

  // ptr<const Type> functype = IRBuilder::getFunctionType(FunctionType::NotVararg,
  //                                                       inttype,
  //                                                       inttype);

  //ptr<GlobalVariable> asym = IRBuilder::GetOrCreateGlobalVariable("a", inttype);

  //ptr<GlobalVariable> bsym = IRBuilder::GetOrCreateGlobalVariable("b", inttype);

  //ptr<Function> fsym = IRBuilder::GetOrCreateFunction("foo", functype);

  Builder::GlobalVariableTerminal a = {{"a"}};
  Builder::GlobalVariableTerminal b = {{"b"}};

  Builder::FunctionTerminal foo = {{"foo"}};

  auto code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(module,
      func["testfunc"].type[void_()] [
        b = a,
        foo(b)
      ]
    );

  mirv::PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
