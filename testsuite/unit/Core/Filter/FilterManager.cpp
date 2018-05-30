// Test building of variables.
//
// STDOUT: mdef testmodule {
// STDOUT:    fdecl testfunc void ()
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl a int32 *
// STDOUT:       {
// STDOUT:          allocate int32
// STDOUT:             vref a
// STDOUT:             cref int64 1
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/Filter/FilterManager.hpp>
#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

namespace Builder = mirv::Builder;

using Builder::var;
using Builder::func;
using Builder::module;
using Builder::void_;
using Builder::int_;

int main(void)
{
  auto code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        func["testfunc"].type[void_()] [
  	  var["a"].type[int_(32)]
        ]
      ]
    );

  mirv::FilterManager Filters;
  Filters.add(mirv::ptr<mirv::PrintFilter>(new mirv::PrintFilter(std::cout)));
  Filters.run(code);

  return(0);
}
