// Test building of pointers to array elements.
//
// STDOUT: fdef testfunc {
// STDOUT:    vdecl a (cref int64 5 x (cref int64 4 x (cref int64 3 x int32))) *
// STDOUT:    vdecl b int32 * *
// STDOUT:    {
// STDOUT:       allocate (cref int64 5 x (cref int64 4 x (cref int64 3 x int32)))
// STDOUT:          vref a
// STDOUT:          cref int64 1
// STDOUT:       allocate int32 *
// STDOUT:          vref b
// STDOUT:          cref int64 1
// STDOUT:       store
// STDOUT:          vref b
// STDOUT:          &
// STDOUT:             vref a
// STDOUT:             cref int64 0
// STDOUT:             cref int32 2
// STDOUT:             cref int32 0
// STDOUT:             cref int32 1
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/FunctionGrammar.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::IRBuilder;
using mirv::Symbol;
using mirv::Module;
using mirv::Function;
using mirv::ptr;
using mirv::PrintFilter;

namespace Builder = mirv::Builder;

using Builder::func;
using Builder::var;
using Builder::void_;
using Builder::int_;

int main(void)
{
  ptr<Module> module = IRBuilder::GetOrCreateModule("testmodule");

  Builder::VariableTerminal a = {{"a"}};
  Builder::VariableTerminal b = {{"b"}};

  auto code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(
      module, ptr<Function>(),
      func["testfunc"].type[void_()] [
        var["a"].type[int_(32)[boost::proto::lit(5),4,3]],
        var["b"].type[*int_(32)],

        b = &a[boost::proto::lit(2),0,1]
      ]
    );

  mirv::PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
