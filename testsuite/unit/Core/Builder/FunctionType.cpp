// Test building of function types.
//
// STDOUT: mdef testmodule {
// STDOUT:    fdecl testfunc void (int32, int16)
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl a int32 *
// STDOUT:       {
// STDOUT:          allocate int32
// STDOUT:             vref a
// STDOUT:             cref int64 1
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::Module;
using mirv::Function;
using mirv::Type;
using mirv::FunctionType;
using mirv::ptr;
using mirv::PrintFilter;

namespace Builder = mirv::Builder;

using Builder::func;
using Builder::module;
using Builder::var;
using Builder::void_;
using Builder::int_;

int main(void)
{
  auto code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        func["testfunc"].type[void_(int_(32), int_(16))] [
          var["a"].type[int_(32)]
        ]
      ]
    );

  PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
