// Test building of function calls as expressions.
//
// STDOUT: mdef testmodule {
// STDOUT:    gvdecl a int32
// STDOUT:    gvdecl b int32
// STDOUT:    fdecl foo int32 (int32)
// STDOUT:    fdecl testfunc void ()
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl c int32 *
// STDOUT:       vdecl d int32 *
// STDOUT:       vdecl __ct0__ int32
// STDOUT:       {
// STDOUT:          allocate int32
// STDOUT:             vref c
// STDOUT:             cref int64 1
// STDOUT:          allocate int32
// STDOUT:             vref d
// STDOUT:             cref int64 1
// STDOUT:          call
// STDOUT:             fref foo
// STDOUT:             vref __ct0__
// STDOUT:             load
// STDOUT:                vref c
// STDOUT:          store
// STDOUT:             vref d
// STDOUT:             +
// STDOUT:                load
// STDOUT:                   vref c
// STDOUT:                vref __ct0__
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/IR/ControlStructure.hpp>
#include <mirv/Core/IR/Producers.hpp>
#include <mirv/Core/Builder/Builder.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Core/Builder/Domain.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

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
  Builder::VariableTerminal a = {{"a"}};
  Builder::VariableTerminal b = {{"b"}};
  Builder::VariableTerminal c = {{"c"}};
  Builder::VariableTerminal d = {{"d"}};

  Builder::FunctionTerminal foo = {{"foo"}};

  auto code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
	var[a].type[int_(32)],
	var[b].type[int_(32)],
        func["foo"].type[int_(32) (int_(32))],
        func["testfunc"].type[void_()] [
          var[c].type[int_(32)],
          var[d].type[int_(32)],
          d = c + foo(c)
        ]
      ]
    );

  mirv::PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
