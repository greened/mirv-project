// Test building of function calls combined with another statement.
// STDOUT: fdef testfunc {
// STDOUT:    {
// STDOUT:       store
// STDOUT:          cref int32 * & b
// STDOUT:          load
// STDOUT:             cref int32 * & a
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
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::GlobalVariable;
using mirv::Type;
using mirv::FunctionType;
using mirv::ptr;
using mirv::PrintFilter;
using mirv::IRBuilder;

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

  std::vector<ptr<const Type>> argTypes;
  argTypes.push_back(inttype);

  // auto functype = IRBuilder::getFunctionType(FunctionType::NotVararg,
  //                                            IRBuilder::getVoidType(),
  //                                            inttype);

  // auto asym = IRBuilder::GetOrCreateGlobalVariable("a", inttype);
  // auto bsym = IRBuilder::GetOrCreateGlobalVariable("b", inttype);

  // auto fsym = IRBuilder::GetOrCreateFunction("foo", functype);

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
