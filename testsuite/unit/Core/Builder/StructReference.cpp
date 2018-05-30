// Test building of array types.
//
// STDOUT: mdef testmodule {
// STDOUT:    fdecl testfunc void ()
// STDOUT:    fdef testfunc {
// STDOUT:       vdecl a (int32, (int32, float32, (int32, float32)), int16) *
// STDOUT:       vdecl b int32 *
// STDOUT:       {
// STDOUT:          allocate (int32, (int32, float32, (int32, float32)), int16)
// STDOUT:             vref a
// STDOUT:             cref int64 1
// STDOUT:          allocate int32
// STDOUT:             vref b
// STDOUT:             cref int64 1
// STDOUT:          store
// STDOUT:             vref b
// STDOUT:             cref int32 1
// STDOUT:          store
// STDOUT:             vref b
// STDOUT:             +
// STDOUT:                load
// STDOUT:                   vref b
// STDOUT:                load
// STDOUT:                   &
// STDOUT:                      &
// STDOUT:                         &
// STDOUT:                            vref a
// STDOUT:                            cref int64 0
// STDOUT:                            cref int32 1
// STDOUT:                         cref int64 0
// STDOUT:                         cref int32 2
// STDOUT:                      cref int64 0
// STDOUT:                      cref int32 0
// STDOUT:       }
// STDOUT:    }
// STDOUT: }

#include <mirv/Core/IR/Module.hpp>
#include <mirv/Core/IR/Function.hpp>
#include <mirv/Core/IR/GlobalVariable.hpp>
#include <mirv/Core/IR/Type.hpp>
#include <mirv/Core/Builder/ModuleGrammar.hpp>
#include <mirv/Core/Builder/SymbolGrammar.hpp>
#include <mirv/Core/Builder/Translate.hpp>
#include <mirv/Filter/Snapshot/Print/Print.hpp>

namespace Builder = mirv::Builder;

using Builder::module;
using Builder::func;
using Builder::var;
using Builder::void_;
using Builder::int_;
using Builder::float_;
using Builder::struct_;

int main(void)
{
  Builder::VariableTerminal a = {{"a"}};
  Builder::VariableTerminal b = {{"b"}};

  auto code =
    Builder::translateWithGrammar<Builder::ModuleBuilder>(
      module["testmodule"] [
        struct_["A"] [
          int_(32),
          struct_["B"] [
            int_(32),
            float_(32),
            struct_["C"] [
              int_(32),
              float_(32)
            ]
          ],
          int_(16)
        ],
        func["testfunc"].type[void_()] [
          var["a"].type["A"],
          var["b"].type[int_(32)],

          b = 1,
          b = b + a[1][2][0]
        ]
      ]
    );

  mirv::PrintFilter print(std::cout);

  print.run(code);

  return(0);
}
