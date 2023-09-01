// Test building of struct types.
//
// STDOUT: mdef testmodule {
// STDOUT:    gvdecl a (int32, float32, (int32), int16)
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

using mirv::ptr;
using mirv::PrintFilter;

namespace Builder = mirv::Builder;

using Builder::int_;
using Builder::float_;
using Builder::struct_;
using Builder::module;
using Builder::var;

int main(void)
{
  auto code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        struct_["teststruct"] [
          int_(32),
          float_(32),
          struct_["testnest"] [
            int_(32)
          ],
          int_(16)
        ],
        var["a"].type["teststruct"]
      ]
    );

  PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
