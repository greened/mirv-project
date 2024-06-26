// Test building of function declaration.
//
// STDOUT: mdef testmodule {
// STDOUT:    gvdecl g int32
// STDOUT:    fdecl foo void (int32)
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

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
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

using Builder::var;
using Builder::func;
using Builder::module;
using Builder::void_;
using Builder::int_;
using Builder::float_;

int main(void)
{
  auto code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        var["g"].type[int_(32)],
        func["foo"].type[void_(int_(32))],
        func["testfunc"].type[void_()] [
  	  var["a"].type[int_(32)]
        ]
      ]
    );

  PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
