// Test building of constants.
//
// STDOUT: mdef testmodule {
// STDOUT:    fdecl testfunc void ()
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl i int32 *
// STDOUT:       {
// STDOUT:          allocate int32
// STDOUT:             vref i
// STDOUT:             cref int64 1
// STDOUT:          store
// STDOUT:             vref i
// STDOUT:             cref int32 0
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Constant.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Control.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Module;
using mirv::Function;
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

int main(void)
{
  Builder::VariableTerminal i = {{"i"}};

  auto code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        func["testfunc"].type[void_()] [
          var[i].type[int_(32)],
          i = 0
        ]
      ]
    );

  PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
