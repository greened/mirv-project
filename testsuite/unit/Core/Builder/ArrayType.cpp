// Test building of array types.
//
// STDOUT: fdef testfunc {
// STDOUT:    vdecl a (cref int64 5 x (cref int64 4 x (cref int64 3 x int32))) *
// STDOUT:    {
// STDOUT:       allocate (cref int64 5 x (cref int64 4 x (cref int64 3 x int32)))
// STDOUT:          vref a
// STDOUT:          cref int64 1
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

namespace Builder = mirv::Builder;

using Builder::func;
using Builder::var;
using Builder::void_;
using Builder::int_;

int main(void)
{
  auto module = mirv::IRBuilder::GetOrCreateModule("testmodule");

  auto code =
    Builder::translateWithGrammar<Builder::FunctionBuilder>(
      module, mirv::ptr<mirv::Function>(),
      func["testfunc"].type[void_()] [
        var["a"].type[int_(32)[boost::proto::lit(5),4,3]]
      ]
    );

  mirv::PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
